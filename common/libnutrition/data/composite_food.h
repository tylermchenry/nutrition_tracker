/*
 * composite_food.h - Data object for a food made up of one or more other foods
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

#ifndef COMPOSITE_FOOD_H_
#define COMPOSITE_FOOD_H_

#include "food_collection.h"
#include <QDate>

/* A CompositeFood is a food made up of one or more other foods, which
 * functions logically as a its own food (as opposed to a FoodCollection, which
 * does not logically function in this way).
 *
 * The composite food class is the realization of the idea of a food made from a
 * recipe. The recipe calls for some number of other foods, some of which may
 * be raw or pre-packaged with USDA nutrition data available, and some of which
 * may themselves be composite foods that were prepared previously. The
 * rationale behind the composite food concept is that it makes much more sense
 * for a user to be able to enter "home-made blueberry bread" as a food into
 * the tracker rather than entering separately "flour", "salt", "baking soda",
 * "sugar", "eggs", "blueberries", etc, because when the user consumes the
 * bread, he or she is thinking about it in terms of being a single food, not
 * in terms of a collection of its individual ingredients.
 *
 * The only practical difference between a CompositeFood object and a
 * FoodCollection object is that CompositeFood objects are database-backed,
 * whereas pure FoodCollection objects are by definition temporary.
 *
 * Note that CompositeFoods are restricted to containing SingleFoods and other
 * CompositeFoods. There's nothing in the code that will prevent the illogical
 * insertion of a Meal or a FoodCollection into a CompositeFood, but such
 * objects cannot be saved to the database, and so will not be persisted.
 */
class CompositeFood : virtual public FoodCollection
{
  public:

    // Definitions to make this class cacheable with DataCache
    typedef int cache_key_type;
    typedef CompositeFood cache_object_type;
    static const bool cache_strong = false;

    static QSharedPointer<CompositeFood> createNewCompositeFood
      (const QSharedPointer<const CompositeFood>& copy =
       QSharedPointer<const CompositeFood>());

    static QSharedPointer<CompositeFood> createNewNonceCompositeFood
      (const FoodAmount& basisAmount = FoodAmount());

    static QSharedPointer<CompositeFood> getCompositeFood(int id);

    static QMultiMap<QString, int> getFoodsForUser(int userId);

    static QString generateExpirySuffix
      (const QDate& creation, const QDate& expiry);

    virtual ~CompositeFood() {};

    virtual int getCompositeFoodId() const = 0;

    virtual QDate getCreationDate() const = 0;

    virtual QDate getExpiryDate() const = 0;

    virtual void setCreationDate(const QDate& date) = 0;

    virtual void setExpiryDate(const QDate& date) = 0;

  protected:

    virtual QSharedPointer<Food> getCanonicalSharedPointer() const;

  private:

    static QMap<int, QWeakPointer<CompositeFood> > compositeFoodCache;
};

#endif /* COMPOUND_FOOD_H_ */
