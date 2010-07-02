/*
 * meal.cpp
 * Part of nutrition_tracker
 *
 *  Created on: May 22, 2010
 *      Author: Tyler McHenry <tyler@nerdland.net>
 */

#include "meal_impl.h"
#include "libnutrition/data/data_cache.h"
#include "libnutrition/data/single_food.h"
#include "libnutrition/data/composite_food.h"
#include <QDebug>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlRecord>
#include <QtSql/QSqlField>
#include <QtSql/QSqlError>

MealImpl::MealImpl(int id, int creatorId, const QString& name, int ownerId,
                      const QDate& date, const QList<FoodComponent>& components,
                      int temporaryId)
  : FoodCollectionImpl
      ((temporaryId >= 0 ? "TMPMEAL_" : "MEAL_") + QString::number(id) + "_" +
       QString::number(ownerId) + "_" + date.toString(Qt::ISODate),
       name, ownerId, components, 0, 0, 0, 1),
    id(id), creatorId(creatorId), date(date), temporaryId(temporaryId)
{
  if (!isTemporary() && needsToBeReSaved()) {
    saveToDatabase();
  }
}

MealImpl::MealImpl(int id, int creatorId, const QString& name, int ownerId,
                      const QDate& date, int temporaryId)
  : FoodCollectionImpl
      ((temporaryId >= 0 ? "TMPMEAL_" : "MEAL_") + QString::number(id) + "_" +
       QString::number(ownerId) + "_" + date.toString(Qt::ISODate),
       name, ownerId, 0, 0, 0, 1),
    id(id), creatorId(creatorId), date(date), temporaryId(temporaryId)
{
}

MealImpl::~MealImpl()
{
}

QSharedPointer<Meal> MealImpl::getTemporaryClone() const
{
  QSharedPointer<Meal> tempClone = Meal::createTemporaryMeal(getOwnerId(), getDate(), getMealId());
  tempClone->replaceWith(getCanonicalSharedPointerToCollection());
  return tempClone;
}

void MealImpl::saveToDatabase()
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
    query.bindValue(":userId", getOwnerId());
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

void MealImpl::deleteFromDatabase()
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
  query.bindValue(":userId", getOwnerId());
  query.bindValue(":mealDate", date);

  if (!query.exec()) {
    qDebug() << "Failed to delete meal: " << query.lastError();
  } else {
    clearComponents();
    deleteRemovedNonceFoods();
    DataCache<Meal>::getInstance().remove(getMealIdTuple());
  }
}

