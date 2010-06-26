/*
 * food_tree_component_item.cpp
 *
 *  Created on: May 26, 2010
 *      Author: tyler
 */

#include "food_tree_component_item.h"
#include "food_tree_amount_item.h"
#include "food_component_context_menu.h"
#include "data/food.h"
#include "data/food_collection.h"
#include <QDebug>

FoodTreeComponentItem::FoodTreeComponentItem
  (const QAbstractItemModel* model, const FoodComponent& component, FoodTreeItem* parent)
  : FoodTreeItem(model, parent), component(component)
{
  qDebug() << "Creating food tree component item for component "
           << component.getFoodAmount().getFood()->getName()
           << " in " << component.getContainingCollection()->getName();

  if (component.getFoodAmount().isDefined()) {
    QVector<FoodAmount> scaledComponentAmounts = component.getFoodAmount().getScaledComponents();
    for (QVector<FoodAmount>::const_iterator i = scaledComponentAmounts.begin();
        i != scaledComponentAmounts.end(); ++i)
    {
      addChild(new FoodTreeAmountItem(model, *i, this));
    }
  }
}

FoodTreeComponentItem::~FoodTreeComponentItem()
{
}

FoodContextMenu* FoodTreeComponentItem::getContextMenu()
{
  return new FoodComponentContextMenu(getIndex(), &component);
}

void FoodTreeComponentItem::changeAmount(const FoodAmount& amount)
{
  component = component.getContainingCollection()->changeComponentAmount(component, amount);
}

FoodAmount FoodTreeComponentItem::getFoodAmount() const
{
  return component.getFoodAmount();
}

QString FoodTreeComponentItem::getName() const
{
  const FoodAmount& amt = component.getFoodAmount();
  if (amt.isDefined()) {
    return amt.getFood()->getDisplayName() +
     ((!amt.includesRefuse() && amt.getFood()->getPercentRefuse() > 0) ?
       " [edible part only]" : "");
  } else {
    return "(Undefined Component)";
  }
}

void FoodTreeComponentItem::remove()
{
  qDebug() << "Removing component " << component.getFoodAmount().getFood()->getName()
           << " from " << component.getContainingCollection()->getName();
  component.getContainingCollection()->removeComponent(component);
}
