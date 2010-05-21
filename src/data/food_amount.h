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
#include "data/amount.h"
#include "data/nutrient_amount.h"

class Food; // Forward decl

class FoodAmount : public Amount<Food, FoodAmount>
{
  public:

    explicit FoodAmount(const QSharedPointer<const Food>& food = QSharedPointer<const Food>(),
                        double amount = 0,
                        const QSharedPointer<const Unit>& unit = QSharedPointer<const Unit>());

    virtual ~FoodAmount();

    inline QSharedPointer<const Food> getFood() const { return getSubstance(); }

    QMap<QString, NutrientAmount> getScaledNutrients() const;

    QVector<FoodAmount> getScaledComponents() const;

  private:

    virtual QString getSubstanceName(bool plural) const;
};

#endif /* FOOD_AMOUNT_H_ */
