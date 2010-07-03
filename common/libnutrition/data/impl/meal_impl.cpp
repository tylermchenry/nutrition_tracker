/*
 * meal.cpp
 * Part of nutrition_tracker
 *
 *  Created on: May 22, 2010
 *      Author: Tyler McHenry <tyler@nerdland.net>
 */

#include "meal_impl.h"
#include "libnutrition/backend/back_end.h"
#include <QDebug>

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
  BackEnd::getBackEnd()->storeMeal
    (getCanonicalSharedPointer().dynamicCast<Meal>());
}

void MealImpl::deleteFromDatabase()
{
  BackEnd::getBackEnd()->deleteMeal
    (getCanonicalSharedPointer().dynamicCast<Meal>());
}

