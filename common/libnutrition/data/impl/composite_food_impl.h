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

#ifndef COMPOSITE_FOOD_IMPL_H_
#define COMPOSITE_FOOD_IMPL_H_

#include "food_collection_impl.h"
#include "libnutrition/data/composite_food.h"
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
class CompositeFoodImpl
  : public FoodCollectionImpl, virtual public CompositeFood
{
  public:

    CompositeFoodImpl(int id, const QString& name, int ownerId,
                        const QList<FoodComponent>& components,
                        double weightAmount, double volumeAmount,
                        double quantityAmount, double servingAmount,
                        QDate creationDate = QDate::currentDate(),
                        QDate expiryDate = QDate(),
                        bool nonce = false);

    CompositeFoodImpl(int id, const QString& name, int ownerId,
                        double weightAmount, double volumeAmount,
                        double quantityAmount, double servingAmount,
                        QDate creationDate = QDate::currentDate(),
                        QDate expiryDate = QDate(),
                        bool nonce = false);

    // Default or "Copy" constructor. If a food is passed in to copy, the
    // attributes of the food object are copied, but the constructed food
    // is still assigned a new, temporary ID.
    CompositeFoodImpl(const QSharedPointer<const CompositeFood>& copy =
                        QSharedPointer<const CompositeFood>());

    virtual ~CompositeFoodImpl();

    virtual QString getDisplayName() const;

    virtual inline int getCompositeFoodId() const { return id; }

    virtual inline QDate getCreationDate() const { return creationDate; }

    virtual inline QDate getExpiryDate() const { return expiryDate; }

    virtual void setCreationDate(const QDate& date);

    virtual void setExpiryDate(const QDate& date);

    virtual void saveToDatabase();

    virtual void deleteFromDatabase();

    virtual inline bool isNonce() const { return nonce; }

    virtual QSharedPointer<Food> cloneNonce() const;

  protected:

    virtual inline void setNonce(bool nonce) { this->nonce = nonce; }

  private:

    void validateExpiryDate();

    int id;
    bool nonce;
    QDate creationDate;
    QDate expiryDate;

    static int tempId;
};

#endif /* COMPOSITE_FOOD_IMPL_H_ */
