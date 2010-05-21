/*
 * food_tree_item.cpp
 *
 *  Created on: May 21, 2010
 *      Author: tyler
 */

#include "food_tree_item.h"
#include <QVariant>
#include "data/food.h"

FoodTreeItem::FoodTreeItem(const FoodAmount& foodAmount, FoodTreeItem* parentItem)
  : foodAmount(foodAmount), parentItem(parentItem)
{
  if (foodAmount.isDefined()) {
    QVector<FoodAmount> components = foodAmount.getScaledComponents();
    for (QVector<FoodAmount>::const_iterator i = components.begin(); i != components.end(); ++i) {
      addChild(*i);
    }
  }
}

FoodTreeItem::~FoodTreeItem()
{
  qDeleteAll(childItems);
}

void FoodTreeItem::addChild(const FoodAmount& foodAmount)
{
  childItems.append(new FoodTreeItem(foodAmount, this));
}

FoodTreeItem* FoodTreeItem::child(int row)
{
  return childItems.value(row);
}

int FoodTreeItem::childCount() const
{
  return childItems.count();
}

int FoodTreeItem::columnCount() const
{
  return 3; // Name, amount, and calories, for testing so far
}

QVariant FoodTreeItem::data(int column) const
{
  if (foodAmount.isDefined()) {
    if (column == 0) {
      return foodAmount.getFood()->getName();
    } else if (column == 1) {
      return QString::number(foodAmount.getAmount()) + " " + foodAmount.getUnit()->getAbbreviation();
    } else if (column == 2) {
      // TODO: Get rid of hard-coded ID when done testing
      NutrientAmount calorieAmount = foodAmount.getScaledNutrients()["208"];
      if (calorieAmount.isDefined()) {
        return QString::number(calorieAmount.getAmount()) + " " + calorieAmount.getUnit()->getAbbreviation();
      } else {
        return 0;
      }
    }
    return QVariant();
  } else {
    if (column == 0) {
      return "(Undefined)";
    }
    return QVariant();
  }
}

int FoodTreeItem::row() const
{
  if (parentItem) {
      return parentItem->childItems.indexOf(const_cast<FoodTreeItem*>(this));
  }

  return 0;
}

FoodTreeItem* FoodTreeItem::parent()
{
  return parentItem;
}
