/*
 * meal.cpp
 * Part of nutrition_tracker
 *
 *  Created on: May 22, 2010
 *      Author: Tyler McHenry <tyler@nerdland.net>
 */

#include "meal.h"
#include "impl/meal_impl.h"
#include "data_cache.h"
#include "libnutrition/backend/back_end.h"
#include "single_food.h"
#include "composite_food.h"
#include <QDebug>

int Meal::nextTemporaryId = 0;

QMap<int, QString> Meal::getAllMealNames(int creatorId, bool includeGenerics)
{
  return BackEnd::getBackEnd()->loadAllMealNamesForUser(creatorId, includeGenerics);
}

QSharedPointer<Meal> Meal::createTemporaryMeal(int userId, const QDate& date, int mealId)
{
  QSharedPointer<Meal> meal
    (new MealImpl(mealId, userId,
                  getAllMealNames()[mealId],
                  userId, date, QList<FoodComponent>(), nextTemporaryId++));

  DataCache<TemporaryMeal>::getInstance().insert(meal->getTemporaryId(), meal);
  return meal;
}

QSharedPointer<Meal> Meal::getOrCreateMeal(int userId, const QDate& date, int mealId)
{
  QSharedPointer<Meal> meal = getMeal(userId, date, mealId);

  if (meal == NULL) {
    QSharedPointer<Meal> newMeal
      (new MealImpl(mealId, userId,
                    getAllMealNames()[mealId],
                    userId, date, QList<FoodComponent>()));
    DataCache<Meal>::getInstance().insert(newMeal->getMealIdTuple(), newMeal);
    meal = newMeal;
  }

  return meal;
}

QSharedPointer<Meal> Meal::getMeal(int userId, const QDate& date, int mealId)
{
  if (DataCache<Meal>::getInstance().contains(MealIdTuple(userId, date, mealId)))
  {
    return DataCache<Meal>::getInstance().get(MealIdTuple(userId, date, mealId));
  }
  else
  {
    return BackEnd::getBackEnd()->loadMeal(userId, date, mealId);
  }
}

QVector<QSharedPointer<Meal> > Meal::getMealsForDay(int userId, const QDate& date)
{
  return BackEnd::getBackEnd()->loadAllMealsForDay(userId, date).toVector();
}

QSharedPointer<Food> Meal::getCanonicalSharedPointer() const
{
  if (isTemporary()) {
    return DataCache<TemporaryMeal>::getInstance().get(getTemporaryId());
  } else {
    return DataCache<Meal>::getInstance().get(getMealIdTuple());
  }
}

Meal::MealIdTuple::MealIdTuple(int uid, const QDate& d, int mid)
  : userId(uid), date(d), mealId(mid)
{
}

bool Meal::MealIdTuple::operator< (const MealIdTuple& rhs) const
{
  return userId < rhs.userId ||
    (userId == rhs.userId && date < rhs.date) ||
    (userId == rhs.userId && date == rhs.date && mealId < rhs.mealId);
}


