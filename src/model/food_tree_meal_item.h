/*
 * food_tree_meal_item.h
 *
 *  Created on: May 26, 2010
 *      Author: tyler
 */

#ifndef FOOD_TREE_MEAL_ITEM_H_
#define FOOD_TREE_MEAL_ITEM_H_

#include "food_tree_item.h"
#include "food_tree_component_item.h"
#include "data/meal.h"

class FoodTreeMealItem : public FoodTreeItem
{
  public:

    explicit FoodTreeMealItem(const QSharedPointer<Meal>& meal, FoodTreeItem* parent = NULL);

    virtual ~FoodTreeMealItem();

    FoodTreeComponentItem* addComponent(const FoodComponent& component);

  protected:

    virtual FoodAmount getFoodAmount() const;

    virtual QString getName() const;

    virtual bool showAmount() const { return false; }

  private:

    QSharedPointer<Meal> meal;

};

#endif /* FOOD_TREE_MEAL_ITEM_H_ */
