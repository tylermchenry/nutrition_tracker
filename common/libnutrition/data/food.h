/*
 * food.h - Data object for something edible
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

#ifndef FOOD_H_
#define FOOD_H_

#include <QVector>
#include <QMap>
#include <QSet>
#include <QString>
#include <QSharedPointer>
#include "libnutrition/data/food_amount.h"
#include "libnutrition/data/user.h"
#include "libnutrition/data/unit.h"
#include "libnutrition/data/specialized_unit.h"

class FoodComponent; // Forward decl
class FoodData; // Forward decl

/* The Food class is the parent class for all edible objects: single foods,
 * composite foods, collections, and meals. The concept of the Food class is
 * very basic: something that can be eaten.
 *
 * Something that can be eaten has a collection of nutrients that it contains,
 * and if it is made up of more than one thing, it has components that make it
 * up. Each Food also has a name and a unique identification string used for
 * quick comparisons.
 *
 * All Foods have a "base amount" that defines the amount of the food that its
 * collection of nutrients describes. A food may have multiple "base amounts"
 * for multiple dimensions (weight, volume, quantity, servings), but may only
 * have a single base amount for each dimension. The getBaseAmount() accessor
 * returns a FoodAmount object with the special property that the FoodAmount's
 * scaled nutrients are equal to the Food's contained nutrient collection
 * (because no scaling is required).
 *
 * This is an abstract base class and cannot be instantiated.
 */
class Food
{
  public:

    virtual ~Food() {};

    virtual QString getId() const = 0;

    virtual QString getName() const = 0;

    virtual int getOwnerId() const = 0;

    virtual QSharedPointer<const User> getOwner() const = 0;

    virtual QString getDisplayName() const = 0;

    virtual QList<Unit::Dimensions::Dimension>
      getValidDimensions() const = 0;

    virtual FoodAmount getBaseAmount() const = 0;

    virtual FoodAmount getBaseAmount
      (Unit::Dimensions::Dimension dimension) const = 0;

    virtual void setBaseAmount
      (double amount, const QSharedPointer<const Unit>& unit) = 0;

    virtual QList<FoodComponent> getComponents() const = 0;

    virtual QVector<FoodAmount> getComponentAmounts() const = 0;

    virtual double getPercentRefuse() const = 0;

    virtual QString getRefuseDescription() const = 0;

    virtual QMap<QString, NutrientAmount> getNutrients() const = 0;

    virtual QVector<QSharedPointer<const SpecializedUnit> >
      getAllSpecializedUnits() const = 0;

    virtual QSharedPointer<const SpecializedUnit>
      getSpecializedUnit(int) const = 0;

    virtual void setName(const QString& name) = 0;

    virtual NutrientAmount getCaloriesFromNutrient
      (const QSharedPointer<const Nutrient>& nutrient) const = 0;

    virtual NutrientAmount getCaloriesFromNutrientName
      (const QString& nutrName) const = 0;

    virtual NutrientAmount getCaloriesFromNutrientId
      (const QString& nutrId) const = 0;

    virtual void saveToDatabase() = 0;

    virtual void deleteFromDatabase() = 0;

    virtual bool isNonce() const = 0;

    virtual QSharedPointer<Food> cloneNonce() const = 0;

    virtual FoodData& serialize(FoodData& fdata) const = 0;

  protected:

    virtual QSharedPointer<Food>
      getCanonicalSharedPointer() const = 0;

    virtual const QMap<Unit::Dimensions::Dimension, double>&
      getBaseAmounts() const = 0;

    virtual void setNonce(bool nonce) = 0;

  private:

    friend class FoodImpl;
};

#endif /* FOOD_H_ */
