/*
 * food_amount.h
 *
 *  Created on: May 14, 2010
 *      Author: tmchenry
 */

#ifndef FOOD_AMOUNT_H_
#define FOOD_AMOUNT_H_

#include <QMap>
#include <QString>
#include "data/nutrient_amount.h"
#include "data/amount.h"

class Food; // Forward decl

class FoodAmount : public Amount<Food, FoodAmount>
{
  public:

    explicit FoodAmount(const Food* food = NULL, double amount = 0,
                        const Unit* unit = NULL);

    virtual ~FoodAmount();

    inline const Food* getFood() const { return getSubstance(); }

    QMap<QString, NutrientAmount> getScaledNutrients() const;

  private:

    virtual QString getSubstanceName(bool plural) const;
};

#endif /* FOOD_AMOUNT_H_ */
