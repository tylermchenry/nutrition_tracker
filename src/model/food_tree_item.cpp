/*
 * food_tree_item.cpp
 *
 *  Created on: May 21, 2010
 *      Author: tyler
 */

#include "food_tree_item.h"
#include <QVariant>
#include <QDebug>
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

  columnNutrientIds.resize(4);
  columnNutrientIds[0] = Nutrient::getNutrientByName("Calories")->getId();
  columnNutrientIds[1] = Nutrient::getNutrientByName("Total Fat")->getId();
  columnNutrientIds[2] = Nutrient::getNutrientByName("Protein")->getId();
  columnNutrientIds[3] = Nutrient::getNutrientByName("Total Carbohydrate")->getId();
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
  return columnNutrientIds.size() + 2;
}

QVariant FoodTreeItem::data(int column) const
{
  if (foodAmount.isDefined()) {

    if (column == 0) {

      return foodAmount.getFood()->getName();

    } else if (column == 1) {

      return QString::number(foodAmount.getAmount()) + " " + foodAmount.getUnit()->getAbbreviation();

    } else if (column > 1 && column < columnNutrientIds.size()+2) {

      QMap<QString, NutrientAmount> scaledNutrients = foodAmount.getScaledNutrients();

      if (scaledNutrients.contains(columnNutrientIds[column-2])) {

        NutrientAmount nutrientAmount = scaledNutrients[columnNutrientIds[column-2]];

        if (nutrientAmount.isDefined()) {
          return QString::number(nutrientAmount.getAmount()) + " " + nutrientAmount.getUnit()->getAbbreviation();
        }

      }

      return 0;
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
