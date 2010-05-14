/*
 * compound_food.h
 *
 *  Created on: May 14, 2010
 *      Author: tmchenry
 */

#ifndef COMPOUND_FOOD_H_
#define COMPOUND_FOOD_H_

#include "food.h"

class CompoundFood: public Food
{
  public:

    explicit CompoundFood(int id = 0);

    virtual ~CompoundFood();

    virtual QVector<FoodAmount> getComponents() const;

    virtual QMap<QString, NutrientAmount> getNutrients() const;

  private:

    int id;
    QVector<FoodAmount> components;

    static QMap<QString, NutrientAmount>& mergeNutrients
      (QMap<QString, NutrientAmount>& nutrients,
       const QMap<QString, NutrientAmount>& newNutrients);
};

#endif /* COMPOUND_FOOD_H_ */
