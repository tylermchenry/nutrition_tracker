/*
 * food_tree_meal_item.cpp
 *
 *  Created on: May 26, 2010
 *      Author: tyler
 */

#include "food_tree_meal_item.h"
#include "food_tree_component_item.h"

FoodTreeMealItem::FoodTreeMealItem(const QSharedPointer<Meal>& meal, FoodTreeItem* parent)
  : FoodTreeItem(parent), meal(meal)
{
  if (meal) {
    QSet<FoodComponent> components = meal->getComponents();
    for (QSet<FoodComponent>::const_iterator i = components.begin(); i != components.end(); ++i) {
      addComponent(*i);
    }
  }
}

FoodTreeMealItem::~FoodTreeMealItem()
{
}

FoodTreeComponentItem* FoodTreeMealItem::addComponent(const FoodComponent& component)
{
  FoodTreeComponentItem* item = new FoodTreeComponentItem(component, this);
  addChild(item);
  return item;
}

FoodAmount FoodTreeMealItem::getFoodAmount() const
{
  if (meal) {
    return meal->getBaseAmount();
  } else {
    return FoodAmount();
  }
}

QString FoodTreeMealItem::getName() const
{
  if (meal) {
    return meal->getName();
  } else {
    return "(Undefined Meal)";
  }
}
