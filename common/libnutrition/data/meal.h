/*
 * meal.h - Data object for a meal
 *
 * This file is part of Nutrition Tracker.
 *
 * Nutrition Tracker is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Nutrition Tracker is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Nutrition Tracker.  If not, see <http://www.gnu.org/licenses/>.
 *
 * Copyright © 2010 Tyler McHenry <tyler@nerdland.net>
 */

#ifndef MEAL_H_
#define MEAL_H_

#include "food_collection.h"
#include <QDate>
#include <QMap>

/* A meal is a a collection of foods associated with a unique triplet of
 * (meal ID, user, date). A meal ID refers to a particular named meal, e.g.
 * "Breakfast", "Lunch", "Dinner", or other user-defined identifiers. There may
 * be up to one meal of each type per user per day stored in the database.
 *
 * A Meal object may be either database-backed or temporary. A temporary meal
 * may not be saved to the database; it must instead be merged into a database-
 * backed meal, if it needs to be saved. The purpose of a temporary meal is
 * for "staging" creation of or additions to meals, so the user can experiment
 * with different foods in a meal without operating on the live database.
 *
 * It may be slightly confusing that a Meal object has a userId and a
 * creatorUserId. The former describes the user who ate the specific meal
 * described by the object, while the latter describes the user who created the
 * meal. The creatorUserId will be 0 if the meal is a standard pre-set meal
 * like Breakfast, Lunch, or Dinner, and will usually match the userId if it is
 * a custom meal, but this distinction allows for future expansion.
 */
class Meal : virtual public FoodCollection
{
  public:

    struct MealIdTuple
    {
      int userId;
      QDate date;
      int mealId;

      MealIdTuple(int uid, const QDate& d, int mid);

      bool operator< (const MealIdTuple& rhs) const;
    };

    // Definitions to make this class cacheable with DataCache
    typedef MealIdTuple cache_key_type;
    typedef Meal cache_object_type;
    static const bool cache_strong = false;

    static QMap<int, QString> getAllMealNames
      (int creatorId = -1, bool includeGenerics = true);

    static QSharedPointer<Meal> createTemporaryMeal
      (int userId, const QDate& date, int mealId);

    static QSharedPointer<Meal> getOrCreateMeal
      (int userId, const QDate& date, int mealId);

    static QSharedPointer<Meal> getMeal
      (int userId, const QDate& date, int mealId);

    static QVector<QSharedPointer<Meal> > getMealsForDay
      (int userId, const QDate& date);

    static QSharedPointer<Meal> createMealFromQueryResults
      (QSqlQuery& query);

    virtual bool isTemporary() const = 0;

    virtual int getMealId() const = 0;

    MealIdTuple getMealIdTuple() const
      { return MealIdTuple(getOwnerId(), getDate(), getMealId()); }

    virtual int getCreatorId() const = 0;

    virtual QSharedPointer<const User> getCreator() const = 0;

    virtual QDate getDate() const = 0;

    virtual QSharedPointer<Meal> getTemporaryClone() const = 0;

  protected:

    virtual QSharedPointer<Food> getCanonicalSharedPointer() const;

    virtual int getTemporaryId() const = 0;

  private:

    struct TemporaryMeal {

      // This is a dummy struct to force a seperate instantiation of
      // DataCache for temporary meals (as opposed to non-temporary meals)

      // Even though they are not backed by the database, a cache is needed for
      // temporary meals because they still require canonical pointers

      // Definitions to make this class cacheable with DataCache
      typedef int cache_key_type;
      typedef Meal cache_object_type;
      static const bool cache_strong = false;
    };

    static int nextTemporaryId;

    friend class MealImpl;
};

#endif /* MEAL_H_ */
