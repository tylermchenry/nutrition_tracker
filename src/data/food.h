/*
 * food.h
 *
 *  Created on: May 14, 2010
 *      Author: tmchenry
 */

#ifndef FOOD_H_
#define FOOD_H_

#include <QVector>
#include <QMap>
#include <QString>
#include "data/food_amount.h"
#include "data/nutrient_amount.h"
#include "data/unit.h"

class Food
{
  public:

    explicit Food(const QString& id = "");

    virtual ~Food();

    inline QString getId() const { return id; }

    inline FoodAmount getBaseAmount() const { return baseAmount; }

    void setBaseAmount(double amount, const Unit* unit);

    virtual QVector<FoodAmount> getComponents() const;

    virtual QMap<QString, NutrientAmount> getNutrients() const = 0;

  private:

    QString id;
    FoodAmount baseAmount;

};

#endif /* FOOD_H_ */
