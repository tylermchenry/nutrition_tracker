/*
 * food_component.h
 * Part of nutrition_tracker
 *
 *  Created on: May 25, 2010
 *      Author: Tyler McHenry <tyler@nerdland.net>
 */

#ifndef FOOD_COMPONENT_H_
#define FOOD_COMPONENT_H_

#include "food_amount.h"

class FoodComponent
{
  public:

    FoodComponent();

    FoodComponent(int id, const FoodAmount& foodAmount, int order = 0);

    virtual ~FoodComponent();

    int getId() const { return id; }

    const FoodAmount& getFoodAmount() const { return foodAmount; }

    int getOrder() const { return order; }

    bool operator< (const FoodComponent& rhs) const;

    bool operator== (const FoodComponent& rhs) const;

  private:

    int id;
    FoodAmount foodAmount;
    int order;
};

uint qHash(const FoodComponent& component);

#endif /* FOOD_COMPONENT_H_ */