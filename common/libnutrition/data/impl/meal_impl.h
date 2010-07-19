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

#ifndef MEAL_IMPL_H_
#define MEAL_IMPL_H_

#include "food_collection_impl.h"
#include "libnutrition/data/meal.h"
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
class MealImpl : public FoodCollectionImpl, virtual public Meal
{
  public:

    MealImpl(int id, int creatorId, const QString& name, int ownerId,
              const QDate& date, const QList<FoodComponent>& components,
              int temporaryId = -1);

    MealImpl(int id, int creatorId, const QString& name, int ownerId,
              const QDate& date, int temporaryId = -1);

    virtual ~MealImpl();

    virtual inline bool isTemporary() const { return (temporaryId >= 0); }

    virtual inline int getMealId() const { return id; }

    virtual inline int getCreatorId() const { return creatorId; }

    virtual inline QSharedPointer<const User> getCreator() const
      { return User::getUser(creatorId); }

    virtual inline QDate getDate() const { return date; }

    virtual QSharedPointer<Meal> getTemporaryClone() const;

    virtual void saveToDatabase();

    virtual void deleteFromDatabase();

    virtual FoodData& serialize(FoodData& fdata) const;

    virtual MealData serialize() const;

  protected:

    virtual inline int getTemporaryId() const { return temporaryId; }

  private:

    int id;
    int creatorId;
    QDate date;
    int temporaryId;   // If >= 0, then temporary and not backed by database
};

#endif /* MEAL_IMPL_H_ */
