/*
 * food_tree_amount_item.cpp
 *
 *  Created on: May 26, 2010
 *      Author: tyler
 */

#include "food_tree_amount_item.h"
#include "libnutrition/data/food.h"
#include "libnutrition/data/food_collection.h"

FoodTreeAmountItem::FoodTreeAmountItem
  (const QAbstractItemModel* model, const FoodAmount& foodAmount, FoodTreeItem* parent)
  : FoodTreeItem(model, parent), foodAmount(foodAmount)
{
  if (foodAmount.isDefined()) {
    QVector<FoodAmount> scaledComponentAmounts = foodAmount.getScaledComponents();
    for (QVector<FoodAmount>::const_iterator i = scaledComponentAmounts.begin();
        i != scaledComponentAmounts.end(); ++i)
    {
      addChild(new FoodTreeAmountItem(model, *i, this));
    }
  }
}

FoodTreeAmountItem::~FoodTreeAmountItem()
{
}

FoodAmount FoodTreeAmountItem::getFoodAmount() const
{
  return foodAmount;
}

QString FoodTreeAmountItem::getName() const
{
  if (foodAmount.isDefined()) {
    return foodAmount.getFood()->getDisplayName() +
        ((!foodAmount.includesRefuse() && foodAmount.getFood()->getPercentRefuse() > 0) ?
          " [edible part only]" : "");
  } else {
    return "(Undefined Food Amount)";
  }
}
