/*
 * meal.cpp
 * Part of nutrition_tracker
 *
 *  Created on: May 22, 2010
 *      Author: Tyler McHenry <tyler@nerdland.net>
 */

#include "meal.h"
#include <QDebug>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlRecord>
#include <QtSql/QSqlField>
#include <QtSql/QSqlError>
#include "data/single_food.h"
#include "data/composite_food.h"

QMap<int, QMap<QDate, QMap<int, QWeakPointer<Meal> > > > Meal::mealCache;

int Meal::nextTemporaryId = 0;

QMap<int, QWeakPointer<Meal> > Meal::temporaryMealCache;

QMap<int, QString> Meal::getAllMealNames(int creatorUserId, bool includeGenerics)
{
  QSqlDatabase db = QSqlDatabase::database("nutrition_db");
  QSqlQuery query(db);

  QString queryText = "SELECT meal.Meal_Id, meal.Name FROM meal";

  if (creatorUserId >= 0) {
    queryText += " WHERE meal.CreatorUser_Id=:creatorId";
  }

  if (includeGenerics) {
    if (creatorUserId >= 0) {
      queryText += " OR";
    } else {
      queryText += " WHERE";
    }
    queryText += " meal.CreatorUser_Id IS NULL";
  }

   if (!query.prepare(queryText)) {
     qDebug() << "Query prepare failed: " << query.lastError();
   }

   if (creatorUserId >= 0) {
     query.bindValue(":creatorId", creatorUserId);
   }

   QMap<int, QString> mealNames;

   if (query.exec()) {
     qDebug() << "Executed query: " << query.executedQuery();
     while (query.next()) {
       mealNames[query.record().field("Meal_Id").value().toInt()] =
         query.record().field("Name").value().toString();
     }
   } else {
     qDebug() << "Query failed: " << query.lastError();
   }

   return mealNames;
}

QSharedPointer<Meal> Meal::createTemporaryMeal(int userId, const QDate& date, int mealId)
{
  QSharedPointer<Meal> meal
    (new Meal(mealId, userId,
              getAllMealNames()[mealId],
              userId, date, QList<FoodComponent>(), nextTemporaryId++));
  temporaryMealCache[meal->temporaryId] = meal;
  return meal;
}

QSharedPointer<Meal> Meal::getOrCreateMeal(int userId, const QDate& date, int mealId)
{
  QSharedPointer<Meal> meal = getMeal(userId, date, mealId);

  if (meal == NULL) {
    QSharedPointer<Meal> newMeal
      (new Meal(mealId, userId,
                getAllMealNames()[mealId],
                userId, date, QList<FoodComponent>()));
    mealCache[userId][date][mealId] = newMeal;
    meal = newMeal;
  }

  return meal;
}

QSharedPointer<Meal> Meal::getMeal(int userId, const QDate& date, int mealId)
{
  QSqlDatabase db = QSqlDatabase::database("nutrition_db");
  QSqlQuery query(db);

  if (mealCache[userId][date][mealId]) {
    return mealCache[userId][date][mealId].toStrongRef();
  }

  query.prepare("SELECT meal.Meal_Id, meal.CreatorUser_Id, meal.Name, "
                "       meal_link.MealLink_Id, meal_link.User_Id, meal_link.MealDate, "
                "       meal_link.Contained_Type, meal_link.Contained_Id, "
                "       meal_link.Includes_Refuse, meal_link.Magnitude, "
                "       meal_link.IntramealOrder, units.Unit, units.Type, "
                "       units.Name AS UnitName, units.Factor "
                "FROM"
                "        meal "
                "   JOIN meal_link "
                "        ON meal.Meal_Id = meal_link.Meal_Id "
                "   JOIN units "
                "        ON meal_link.Unit = units.Unit "
                "WHERE meal_link.User_Id = :userId "
                "  AND meal_link.MealDate = :date "
                "  AND meal_link.Meal_Id = :mealId "
                "ORDER BY IntramealOrder ASC");

  query.bindValue(":userId", userId);
  query.bindValue(":date", date);
  query.bindValue(":mealId", mealId);

  if (query.exec()) {
    qDebug() << "Executed query: " << query.executedQuery();
    qDebug() << "There are " << query.size() << " foods in this meal";
    return createMealFromQueryResults(query);
  } else {
    qDebug() << "Query failed: " << query.lastError();
    return QSharedPointer<Meal>();
  }
}

QVector<QSharedPointer<Meal> > Meal::getMealsForDay(int userId, const QDate& date)
{
  QSqlDatabase db = QSqlDatabase::database("nutrition_db");
  QSqlQuery query(db);

  query.prepare("SELECT DISTINCT meal_link.Meal_Id "
                "FROM meal_link "
                "WHERE meal_link.User_Id = :userId "
                "  AND meal_link.MealDate = :date ");

  query.bindValue(":userId", userId);
  query.bindValue(":date", date);

  QVector<QSharedPointer<Meal> > meals;

  if (query.exec()) {
    qDebug() << "Executed query: " << query.executedQuery();
    while (query.next()) {
      meals.push_back(getMeal(userId, date, query.record().field("Meal_Id").value().toInt()));
    }
  }

  return meals;
}

QSharedPointer<Meal> Meal::createMealFromQueryResults(QSqlQuery& query)
{
  QSharedPointer<Meal> meal;

  if (query.first()) {

    const QSqlRecord& record = query.record();
    int id = record.field("Meal_Id").value().toInt();
    int userId = record.field("User_Id").value().toInt();
    QDate date = record.field("MealDate").value().toDate();

    if (!mealCache[userId][date][id]) {

      int creatorUserId = -1;

      if (!record.field("CreatorUser_Id").isNull()) {
        creatorUserId = record.field("CreatorUser_Id").value().toInt();
      }

      meal = QSharedPointer<Meal>
        (new Meal(id, creatorUserId,
                  record.field("Name").value().toString(),
                  userId, date));

      mealCache[userId][date][id] = meal;

    } else {
      return mealCache[userId][date][id].toStrongRef();
    }
  }

  if (meal && query.isActive()) {
    query.seek(-1); // Reset to before first record
    meal->setComponents
      (createComponentsFromQueryResults(query, meal, "MealLink_Id", "IntramealOrder"));
    if (meal->needsToBeReSaved()) {
      meal->saveToDatabase();
    }
  } else if (!meal) {
    qDebug() << "Meal was not created!";
  } else {
    qDebug() << "Could not rewind query!";
  }

  return meal;
}

Meal::~Meal()
{
}

Meal::Meal(int id, int creatorUserId, const QString& name, int userId,
           const QDate& date, const QList<FoodComponent>& components,
           int temporaryId)
  : FoodCollection((temporaryId >= 0 ? "TMPMEAL_" : "MEAL_") + QString::number(id) + "_" +
                   QString::number(userId) + "_" + date.toString(Qt::ISODate),
                   name, components, 0, 0, 0, 1),
    id(id), creatorUserId(creatorUserId), userId(userId), date(date), temporaryId(temporaryId)
{
  if (!isTemporary() && needsToBeReSaved()) {
    saveToDatabase();
  }
}

Meal::Meal(int id, int creatorUserId, const QString& name, int userId,
           const QDate& date, int temporaryId)
  : FoodCollection((temporaryId >= 0 ? "TMPMEAL_" : "MEAL_") + QString::number(id) + "_" +
                   QString::number(userId) + "_" + date.toString(Qt::ISODate),
                   name, 0, 0, 0, 1),
    id(id), creatorUserId(creatorUserId), userId(userId), date(date), temporaryId(temporaryId)
{
}

QSharedPointer<Meal> Meal::getTemporaryClone() const
{
  QSharedPointer<Meal> tempClone = Meal::createTemporaryMeal(getUserId(), getDate(), getMealId());
  tempClone->replaceWith(getCanonicalSharedPointerToCollection());
  return tempClone;
}

void Meal::saveToDatabase()
{
  if (isTemporary()) {
    throw std::logic_error("Attempted to save a temporary meal to the database.");
  }

  QSqlDatabase db = QSqlDatabase::database("nutrition_db");
  QSqlQuery query(db);

  // This needs to work either for a new food or an update to an existing food

  QSet<int> removedLinkIds = getRemovedIds();

  for (QSet<int>::const_iterator i = removedLinkIds.begin(); i != removedLinkIds.end(); ++i)
  {
    query.prepare("DELETE FROM meal_link WHERE MealLink_Id=:linkId");

    query.bindValue(":linkId", *i);

    if (!query.exec()) {
      qDebug() << "Failed to delete removed meal item: " << query.lastError();
      return;
    }
  }

  deleteRemovedNonceFoods();

  QList<FoodComponent> components = getComponents();
  for (QList<FoodComponent>::const_iterator i = components.begin(); i != components.end(); ++i)
  {
    if (i->getFoodAmount().getFood()->isNonce()) {
      i->getFoodAmount().getFood()->saveToDatabase();
    }

    query.prepare("INSERT INTO meal_link "
                   "  (MealLink_Id, Meal_Id, User_Id, MealDate, Contained_Type, "
                   "   Contained_Id, Includes_Refuse, Magnitude, Unit, IntramealOrder) "
                   "VALUES "
                   "  (:linkId, :mealId, :userId, :mealDate, :containedType, "
                   "   :containedId, :includesRefuse, :magnitude, :unit, :order) "
                   "ON DUPLICATE KEY UPDATE "
                   "  Includes_Refuse=:includesRefuse2, Magnitude=:magnitude2, "
                   "  Unit=:unit2, IntramealOrder=:order2");

    query.bindValue(":linkId", i->getId() >= 0 ? QVariant(i->getId()) : QVariant());

    query.bindValue(":mealId", id);
    query.bindValue(":userId", userId);
    query.bindValue(":mealDate", date);

    if (!i->getFoodAmount().isDefined()) continue;

    QSharedPointer<const Food> food = i->getFoodAmount().getFood();

    QSharedPointer<const SingleFood> singleFood;
    QSharedPointer<const CompositeFood> compositeFood;

    FoodCollection::ContainedTypes::ContainedType containedType;
    int containedId;

    if ((singleFood = food.dynamicCast<const SingleFood>()) != NULL) {
      containedType = FoodCollection::ContainedTypes::SingleFood;
      containedId = singleFood->getSingleFoodId();
    } else if ((compositeFood = food.dynamicCast<const CompositeFood>()) != NULL) {
      containedType = FoodCollection::ContainedTypes::CompositeFood;
      containedId = compositeFood->getCompositeFoodId();
    } else {
      continue;
    }

    query.bindValue(":containedType", FoodCollection::ContainedTypes::toHumanReadable(containedType));
    query.bindValue(":containedId", containedId);

    query.bindValue(":includesRefuse", i->getFoodAmount().includesRefuse());
    query.bindValue(":magnitude", i->getFoodAmount().getAmount());
    query.bindValue(":unit", i->getFoodAmount().getUnit()->getAbbreviation());
    query.bindValue(":order", i->getOrder());

    query.bindValue(":includesRefuse2", i->getFoodAmount().includesRefuse());
    query.bindValue(":magnitude2", i->getFoodAmount().getAmount());
    query.bindValue(":unit2", i->getFoodAmount().getUnit()->getAbbreviation());
    query.bindValue(":order2", i->getOrder());

    if (!query.exec()) {
      qDebug() << "Failed to save " << food->getName() << " to meal: " << query.lastError();
    } else {
      if (i->getId() < 0) {
        int newId = query.lastInsertId().toInt();
        qDebug() << "Assigned real ID " << newId
                  << " to food component with temp ID " << i->getId();
        replaceComponent
          (*i, FoodComponent(getCanonicalSharedPointerToCollection(),
                             newId, i->getFoodAmount(), i->getOrder()));
      }
    }
  }
}

void Meal::deleteFromDatabase()
{
  if (isTemporary()) {
    throw std::logic_error("Attempted to save a temporary meal from the database.");
  }

  QSqlDatabase db = QSqlDatabase::database("nutrition_db");
  QSqlQuery query(db);

  query.prepare("DELETE FROM meal_link "
                 "WHERE Meal_Id=:mealId AND User_Id=:userId "
                 "      AND MealDate=:mealDate");
  query.bindValue(":mealId", id);
  query.bindValue(":userId", userId);
  query.bindValue(":mealDate", date);

  if (!query.exec()) {
    qDebug() << "Failed to delete meal: " << query.lastError();
  } else {
    clearComponents();
    deleteRemovedNonceFoods();
    mealCache[userId][date][id].clear();
  }
}

QSharedPointer<Food> Meal::getCanonicalSharedPointer() const
{
  if (isTemporary()) {
    return temporaryMealCache[temporaryId].toStrongRef();
  } else {
    return mealCache[userId][date][id].toStrongRef();
  }
}

