/*
 * single_food.h
 *
 *  Created on: May 14, 2010
 *      Author: tmchenry
 */

#ifndef SINGLE_FOOD_H_
#define SINGLE_FOOD_H_

#include "food.h"

class SingleFood: public Food
{
  public:

    explicit SingleFood(int id = 0);

    virtual ~SingleFood();

    virtual QMap<QString, NutrientAmount> getNutrients() const;

  private:

    int id;
    QMap<QString, NutrientAmount> nutrients;
};

#endif /* SINGLE_FOOD_H_ */
