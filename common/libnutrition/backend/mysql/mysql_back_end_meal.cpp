#include "mysql_back_end.h"
#include "libnutrition/data/data_cache.h"
#include "libnutrition/data/impl/meal_impl.h"
#include <QVariant>
#include <QDebug>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlRecord>
#include <QtSql/QSqlField>
#include <QtSql/QSqlError>
#include <stdexcept>

QSharedPointer<Meal> MySQLBackEnd::loadMeal
   (int userId, const QDate& date, int mealId)
{
  QSqlQuery query(db);

  query.prepare("SELECT meal.Meal_Id, meal.CreatorUser_Id, meal.Name, "
                "       meal_link.MealLink_Id, meal_link.User_Id, meal_link.MealDate, "
                "       meal_link.Contained_Type, meal_link.Contained_Id, "
                "       meal_link.Includes_Refuse, meal_link.Magnitude, "
                "       meal_link.Unit, meal_link.IntramealOrder "
                "FROM"
                "        meal "
                "   JOIN meal_link "
                "        ON meal.Meal_Id = meal_link.Meal_Id "
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

QMap<int, QString> MySQLBackEnd::loadAllMealNames()
{
  return loadAllMealNamesForUser(-1, true);
}

QMap<int, QString> MySQLBackEnd::loadAllMealNamesForUser
   (int userId, bool includeGenerics)
{
  QSqlQuery query(db);

  QString queryText = "SELECT meal.Meal_Id, meal.Name FROM meal";

  if (userId >= 0) {
    queryText += " WHERE meal.CreatorUser_Id=:userId";
  }

  if (includeGenerics) {
    if (userId >= 0) {
      queryText += " OR";
    } else {
      queryText += " WHERE";
    }
    queryText += " meal.CreatorUser_Id IS NULL";
  }

   if (!query.prepare(queryText)) {
     qDebug() << "Query prepare failed: " << query.lastError();
   }

   if (userId >= 0) {
     query.bindValue(":userId", userId);
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

QList<QSharedPointer<Meal> > MySQLBackEnd::loadAllMealsForDay
   (int userId, const QDate& date)
{
  QSqlQuery query(db);

  query.prepare("SELECT DISTINCT meal_link.Meal_Id "
                "FROM meal_link "
                "WHERE meal_link.User_Id = :userId "
                "  AND meal_link.MealDate = :date ");

  query.bindValue(":userId", userId);
  query.bindValue(":date", date);

  QList<QSharedPointer<Meal> > meals;

  if (query.exec()) {
    qDebug() << "Executed query: " << query.executedQuery();
    while (query.next()) {
      meals.push_back
        (Meal::getMeal
          (userId, date, query.record().field("Meal_Id").value().toInt()));
    }
  }

  return meals;
}

void MySQLBackEnd::storeMeal(const QSharedPointer<Meal>& meal)
{
  if (!meal) {
    throw std::logic_error("Attempted to delete a NULL meal");
  } else if (meal->isTemporary()) {
    throw std::logic_error("Attempted to save a temporary meal to the database.");
  }

  QSharedPointer<MealImpl> meal_impl = meal.dynamicCast<MealImpl>();

  QSqlQuery query(db);

  // This needs to work either for a new food or an update to an existing food

  QSet<int> removedLinkIds = meal_impl->getRemovedIds();

  for (QSet<int>::const_iterator i = removedLinkIds.begin(); i != removedLinkIds.end(); ++i)
  {
    query.prepare("DELETE FROM meal_link WHERE MealLink_Id=:linkId");

    query.bindValue(":linkId", *i);

    if (!query.exec()) {
      qDebug() << "Failed to delete removed meal item: " << query.lastError();
      return;
    }
  }

  meal_impl->deleteRemovedNonceFoods();

  QList<FoodComponent> components = meal->getComponents();
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

    query.bindValue(":mealId", meal->getMealId());
    query.bindValue(":userId", meal->getOwnerId());
    query.bindValue(":mealDate", meal->getDate());

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
        meal_impl->replaceComponent
          (*i, FoodComponent(meal_impl->getCanonicalSharedPointerToCollection(),
                             newId, i->getFoodAmount(), i->getOrder()));
      }
    }
  }
}

void MySQLBackEnd::deleteMeal(const QSharedPointer<Meal>& meal)
{
  if (!meal) {
    throw std::logic_error("Attempted to delete a NULL meal");
  } else if (meal->isTemporary()) {
    // Silently ignore requests to delete temporaries; it's simpler to do that
    // than to force the client to check whether a food is temporary or not
    // every time it wants to delete something.
    return;
  }

  QSharedPointer<MealImpl> meal_impl = meal.dynamicCast<MealImpl>();

  QSqlQuery query(db);

  query.prepare("DELETE FROM meal_link "
                 "WHERE Meal_Id=:mealId AND User_Id=:userId "
                 "      AND MealDate=:mealDate");
  query.bindValue(":mealId", meal->getId());
  query.bindValue(":userId", meal->getOwnerId());
  query.bindValue(":mealDate", meal->getDate());

  if (!query.exec()) {
    qDebug() << "Failed to delete meal: " << query.lastError();
  } else {
    meal_impl->clearComponents();
    meal_impl->deleteRemovedNonceFoods();
    DataCache<Meal>::getInstance().remove(meal->getMealIdTuple());
  }
}

void MySQLBackEnd::deleteMeal(int userId, const QDate& date, int mealId)
{
  deleteMeal(Meal::getMeal(userId, date, mealId));
}

QSharedPointer<Meal> MySQLBackEnd::createMealFromQueryResults
  (QSqlQuery& query)
{
  QSharedPointer<MealImpl> meal_impl;

  if (query.first()) {

    const QSqlRecord& record = query.record();
    int id = record.field("Meal_Id").value().toInt();
    int userId = record.field("User_Id").value().toInt();
    QDate date = record.field("MealDate").value().toDate();

    int creatorId = -1;

    if (!record.field("CreatorUser_Id").isNull()) {
      creatorId = record.field("CreatorUser_Id").value().toInt();
    }

    meal_impl = QSharedPointer<MealImpl>
      (new MealImpl(id, creatorId,
                    record.field("Name").value().toString(),
                    userId, date));

    DataCache<Meal>::getInstance().insert(meal_impl->getMealIdTuple(), meal_impl);
  }

  if (meal_impl && query.isActive()) {
    query.seek(-1); // Reset to before first record
    meal_impl->setComponents
    (createComponentsFromQueryResults(query, meal_impl, "MealLink_Id", "Intrameal_implOrder"));
    if (meal_impl->needsToBeReSaved()) {
      meal_impl->saveToDatabase();
    }
  } else if (!meal_impl) {
    qDebug() << "Meal was not created!";
  } else {
    qDebug() << "Could not rewind query!";
  }

  return meal_impl;
}
