/*
 * meal.cpp
 * Part of nutrition_tracker
 *
 *  Created on: May 22, 2010
 *      Author: Tyler McHenry <tyler@nerdland.net>
 */

#include "meal_impl.h"
#include "libnutrition/proto/data/data.pb.h"
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

FoodData& MealImpl::serialize(FoodData& fdata) const
{
  *(fdata.add_meals()) = serialize();
  return fdata;
}

MealData MealImpl::serialize() const
{
  MealData mdata;

  mdata.set_userid(getOwnerId());
  mdata.set_date_iso8601(date.toString(Qt::ISODate).toStdString());
  mdata.set_mealid(id);
  mdata.set_name(getName().toStdString());
  mdata.set_creatorid(creatorId);

  for (QMap<int, FoodComponent>::const_iterator i = getRawComponents().begin();
       i != getRawComponents().end(); ++i)
  {
    *(mdata.add_components()) = i.value().serialize();
  }

  return mdata;
}
