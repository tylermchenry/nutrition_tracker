/*
 * food_tree_component_item.cpp
 *
 *  Created on: May 26, 2010
 *      Author: tyler
 */

#include "food_tree_component_item.h"
#include "food_tree_amount_item.h"
#include "data/food.h"

FoodTreeComponentItem::FoodTreeComponentItem(const FoodComponent& component, FoodTreeItem* parent)
  : FoodTreeItem(parent), component(component)
{
  if (component.getFoodAmount().isDefined()) {
    QVector<FoodAmount> scaledComponentAmounts = component.getFoodAmount().getScaledComponents();
    for (QVector<FoodAmount>::const_iterator i = scaledComponentAmounts.begin();
        i != scaledComponentAmounts.end(); ++i)
    {
      addChild(new FoodTreeAmountItem(*i, this));
    }
  }
}

FoodTreeComponentItem::~FoodTreeComponentItem()
{
}

FoodAmount FoodTreeComponentItem::getFoodAmount() const
{
  return component.getFoodAmount();
}

QString FoodTreeComponentItem::getName() const
{
  if (component.getFoodAmount().isDefined()) {
    return component.getFoodAmount().getFood()->getName();
  } else {
    return "(Undefined Component)";
  }
}
