/*
 * composite_food.h
 *
 *  Created on: May 14, 2010
 *      Author: tmchenry
 */

#ifndef COMPOSITE_FOOD_H_
#define COMPOSITE_FOOD_H_

#include "food.h"

class CompositeFood: public Food
{
  public:

    explicit CompositeFood(int id = 0);

    virtual ~CompositeFood();

    virtual QVector<FoodAmount> getComponents() const;

    virtual QMap<QString, NutrientAmount> getNutrients() const;

  private:

    CompositeFood(int id, const QString& name,
                    const QVector<FoodAmount>& components,
                    double weightAmount, double volumeAmount,
                    double quantityAmount, double servingAmount);

    int id;
    QVector<FoodAmount> components;

    static QMap<QString, NutrientAmount>& mergeNutrients
      (QMap<QString, NutrientAmount>& nutrients,
       const QMap<QString, NutrientAmount>& newNutrients);
};

#endif /* COMPOUND_FOOD_H_ */
