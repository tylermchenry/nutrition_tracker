/*
 * food_component.h - Data object for a component of a food collection
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

#ifndef FOOD_COMPONENT_H_
#define FOOD_COMPONENT_H_

#include "food_amount.h"

class FoodCollection; // Forward decl

/* A FoodComponent is a uniquely-identified, ordered component of a
 * particular FoodCollection. Rather than storing FoodAmount objects,
 * FoodCollections store FoodComponents which contain meta-data about ordering
 * and a unique identifier, used to disambiguate similar components.
 *
 * For example, it's entirely legal for a single food collection to contain
 * three components which are all "50g of flour". The unique identifier makes
 * it possible to specify that the second "50g of flour" should be removed,
 * not the first, and not the third. This is important in terms of the GUI
 * when the user might be clicking on a component to delete it. The user expects
 * the component under his or her cursor to be deleted, not an identical-looking
 * component elsewhere in the collection, even if the two operations are in all
 * other ways equivalent.
 *
 * FoodComponent objects are also useful because they contain a pointer back to
 * the collection that they are a part of. That way, a FoodComponent object
 * can be, for example, removed from its collection without having to maintain
 * an external association between components and collections everywhere that
 * this sort of lookup might be needed.
 */
class FoodComponent
{
  public:

    FoodComponent();

    FoodComponent(const QSharedPointer<FoodCollection>& containingCollection,
                    int id, const FoodAmount& foodAmount, int order = 0);

    virtual ~FoodComponent();

    inline QSharedPointer<FoodCollection> getContainingCollection()
      { return containingCollection; }

    inline QSharedPointer<const FoodCollection>
      getContainingCollection() const
        { return containingCollection; }

    int getId() const { return id; }

    const FoodAmount& getFoodAmount() const { return foodAmount; }

    int getOrder() const { return order; }

    bool operator< (const FoodComponent& rhs) const;

    bool operator== (const FoodComponent& rhs) const;

  private:

    QSharedPointer<FoodCollection> containingCollection;

    int id;
    FoodAmount foodAmount;
    int order;
};

uint qHash(const FoodComponent& component);

#endif /* FOOD_COMPONENT_H_ */
