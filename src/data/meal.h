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
class Meal : public FoodCollection
{
  public:

    static QMap<int, QString> getAllMealNames
      (int creatorUserId = -1, bool includeGenerics = true);

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

    virtual ~Meal();

    inline bool isTemporary() const { return (temporaryId >= 0); }

    inline int getMealId() const { return id; }

    inline int getCreatorUserId() const { return creatorUserId; }

    inline int getUserId() const { return userId; }

    inline QDate getDate() const { return date; }

    virtual QSharedPointer<Meal> getTemporaryClone() const;

    virtual void saveToDatabase();

    virtual void deleteFromDatabase();

  protected:

    virtual QSharedPointer<Food> getCanonicalSharedPointer();

    virtual QSharedPointer<const Food> getCanonicalSharedPointer() const;

  private:

    Meal(int id, int creatorUserId, const QString& name, int userId,
         const QDate& date, const QList<FoodComponent>& components,
         int temporaryId = -1);

    Meal(int id, int creatorUserId, const QString& name, int userId,
         const QDate& date, int temporaryId = -1);

    int id;
    int creatorUserId; // TODO: Replace with user object when created
    int userId;        // TODO: Replace with user object when created
    QDate date;
    int temporaryId;   // If >= 0, then temporary and not backed by database

    static QMap<int, QMap<QDate, QMap<int, QWeakPointer<Meal> > > > mealCache;

    static int nextTemporaryId;

    // Even though they are not backed by the database, a cache is needed for
    // temporary meals because they still require canonical pointers
    static QMap<int, QWeakPointer<Meal> > temporaryMealCache;

};

#endif /* MEAL_H_ */
