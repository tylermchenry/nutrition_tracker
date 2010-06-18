/*
 * food_tree_meal_item.cpp
 *
 *  Created on: May 26, 2010
 *      Author: tyler
 */

#include "food_tree_meal_item.h"
#include "food_tree_component_item.h"
#include "meal_context_menu.h"

FoodTreeMealItem::FoodTreeMealItem
  (const QAbstractItemModel* model, const QSharedPointer<Meal>& meal, FoodTreeItem* parent)
  : FoodTreeItem(model, parent), meal(meal)
{
  if (meal) {
    QList<FoodComponent> components = meal->getComponents();
    for (QList<FoodComponent>::const_iterator i = components.begin(); i != components.end(); ++i) {
      addComponent(*i);
    }
  }
}

FoodTreeMealItem::~FoodTreeMealItem()
{
}

FoodContextMenu* FoodTreeMealItem::getContextMenu()
{
  return new MealContextMenu(getIndex(), meal);
}

FoodTreeComponentItem* FoodTreeMealItem::addComponent(const FoodComponent& component)
{
  FoodTreeComponentItem* item = new FoodTreeComponentItem(getModel(), component, this);
  addChild(item);
  componentChildren[component.getId()] = item;
  return item;
}

FoodTreeComponentItem* FoodTreeMealItem::getComponentItem(const FoodComponent& component) const
{
  if (componentChildren.contains(component.getId())) {
    return componentChildren[component.getId()];
  } else {
    return NULL;
  }
}

void FoodTreeMealItem::removeComponent(const FoodComponent& component)
{
  if (componentChildren.contains(component.getId())) {
    delete componentChildren[component.getId()];
    componentChildren.remove(component.getId());
  }
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
