/*
 * food_amount.h - Data object for an amount of a food
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

#ifndef FOOD_AMOUNT_H_
#define FOOD_AMOUNT_H_

#include <QMap>
#include <QString>
#include "libnutrition/data/amount.h"
#include "libnutrition/data/nutrient_amount.h"

class Food; // Forward decl

/* FoodAmount is an Amount where the substance is a Food. Remember that here,
 * Food is the base class of all objects describing edible things including
 * collections, meals, and composite foods, as well as single foods.
 *
 * In addition to the features of the Amount class, the FoodAmount class
 * provides facilities for obtaining scaled measurements of the nutrients and
 * components of the described food. The nutrients and components of a food
 * are recorded in the database (and in the Food-derived object) in terms of
 * the food's "base amount" in a particular dimension. The scaled accessor
 * functions will scale the nutrients and components of the food in proportion
 * to the ratio between the FoodAmount and the food's base amount.
 *
 * For example, if Food A with a base amount of 100g contains 80 Calories, then
 * a FoodAmount describing 50g of Food A should report in its scaled nutrients
 * that this amount of Food A contains 40 Calories.
 *
 * Similarly, if Food B, a composite food with a base amount of 2 Cups contains
 * 1 tsp of vanilla extract as a component, then a FoodAmount describing 3 Cups
 * of Food B should report in its scaled components that this amount of Food B
 * contains 1.5 tsp of vanilla extract.
 */
class FoodAmount : public Amount<Food, FoodAmount>
{
  public:

    explicit FoodAmount(const QSharedPointer<Food>& food =
                              QSharedPointer<Food>(),
                           double amount = 0,
                           const QSharedPointer<const Unit>& unit =
                              QSharedPointer<const Unit>(),
                           bool includesRefuse = true);

    virtual ~FoodAmount();

    virtual double getAmount
      (const QSharedPointer<const Unit>& otherUnit =
       QSharedPointer<const Unit>()) const;

    inline QSharedPointer<const Food> getFood() const
      { return getSubstance(); }

    inline QSharedPointer<Food> getFood()
      { return getSubstance(); }

    // TODO: Prohibit adding, subtracting, etc. between amounts that include
    // refuse and those that do not.
    inline bool includesRefuse() const
      { return bIncludesRefuse; }

    QMap<QString, NutrientAmount> getScaledNutrients() const;

    QVector<FoodAmount> getScaledComponents() const;

    NutrientAmount getScaledCaloriesFromNutrient
      (const QSharedPointer<const Nutrient>& nutr) const;

    NutrientAmount getScaledCaloriesFromNutrientName
      (const QString& nutrName) const;

    NutrientAmount getScaledCaloriesFromNutrientId
      (const QString& nutrId) const;

  private:

    virtual QString getSubstanceName(bool plural) const;

    double getScaleFactor() const;

    bool bIncludesRefuse;
};

#endif /* FOOD_AMOUNT_H_ */
