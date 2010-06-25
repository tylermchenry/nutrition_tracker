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
#include "data/food_amount.h"
#include "data/unit.h"

class FoodComponent; // Forward decl

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

    virtual ~Food();

    inline QString getId() const { return id; }

    inline QString getName() const { return name; }

    virtual inline QString getDisplayName() const { return name; }

    QList<Unit::Dimensions::Dimension> getValidDimensions() const;

    FoodAmount getBaseAmount() const;

    FoodAmount getBaseAmount(Unit::Dimensions::Dimension dimension) const;

    void setBaseAmount(double amount,
                         const QSharedPointer<const Unit>& unit);

    virtual QList<FoodComponent> getComponents() const;

    virtual QVector<FoodAmount> getComponentAmounts() const;

    inline virtual double getPercentRefuse() const
      { return 0; }

    inline virtual QString getRefuseDescription() const
      { return ""; }

    virtual QMap<QString, NutrientAmount> getNutrients() const = 0;

    void setName(const QString& name);

    virtual NutrientAmount getCaloriesFromNutrient
      (const QSharedPointer<const Nutrient>& nutrient) const;

    virtual NutrientAmount getCaloriesFromNutrientName
      (const QString& nutrName) const;

    virtual NutrientAmount getCaloriesFromNutrientId
      (const QString& nutrId) const = 0;

    virtual void saveToDatabase() = 0;

    virtual void deleteFromDatabase() = 0;

    virtual inline bool isNonce() const { return false; }

    // cloneNonce() needs to be re-implemented only if isNonce() == true
    virtual inline QSharedPointer<Food> cloneNonce() const
      { return QSharedPointer<Food>(); }

  protected:

    Food(const QString& id, const QString& name, double weightAmount,
         double volumeAmount, double quantityAmount, double servingAmount);

    Food(const QString& id, const QSharedPointer<const Food>& copy);

    virtual QSharedPointer<Food>
      getCanonicalSharedPointer() const = 0;

    void bindBaseAmount
      (QSqlQuery& query, const QString& placeholder,
       Unit::Dimensions::Dimension dimension) const;

  private:

    QString id;
    QString name;
    QMap<Unit::Dimensions::Dimension, double> baseAmounts;

};

#endif /* FOOD_H_ */
