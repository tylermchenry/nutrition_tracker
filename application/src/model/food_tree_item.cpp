/*
 * food_tree_item.cpp
 *
 *  Created on: May 21, 2010
 *      Author: tyler
 */

#include "food_tree_item.h"
#include <QVariant>
#include <QDebug>
#include "libnutrition/data/food.h"
#include "libnutrition/data/food_collection.h"
#include "menus/food_context_menu.h"
#include "menus/food_amount_context_menu.h"

FoodTreeItem::FoodTreeItem(const QAbstractItemModel* model, FoodTreeItem* parentItem)
  : model(model), parentItem(parentItem)
{
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
  if (column == 0 && showName()) {

    return getName();

  } else {

    FoodAmount foodAmount = getFoodAmount();

    if (foodAmount.isDefined()) {

      if (column == 1 && showAmount()) {

        return QString::number(foodAmount.getAmount(), 'f', 1) + " " + foodAmount.getUnit()->getAbbreviation();

      } else if (column > 1 && column < columnNutrientIds.size()+2 && showNutrients()) {

        QMap<QString, NutrientAmount> scaledNutrients = foodAmount.getScaledNutrients();

        if (scaledNutrients.contains(columnNutrientIds[column-2])) {

          NutrientAmount nutrientAmount = scaledNutrients[columnNutrientIds[column-2]];

          if (nutrientAmount.isDefined()) {

            int decimalPlaces;

            if (nutrientAmount.getNutrient() == Nutrient::getNutrientByName("Calories")) {
              decimalPlaces = 0;
            } else {
              decimalPlaces = 1;
            }

            return QString::number(nutrientAmount.getAmount(), 'f', decimalPlaces) + " " +
                nutrientAmount.getUnit()->getAbbreviation();
          }
        }
      }
    }
  }

  return QVariant();
}

int FoodTreeItem::row() const
{
  if (parentItem) {
    return parentItem->childItems.indexOf(const_cast<FoodTreeItem*>(this));
  }

  return 0;
}

int FoodTreeItem::depth() const
{
  if (parentItem) {
    return parentItem->depth()+1;
  }

  return 0;
}

FoodTreeItem* FoodTreeItem::parent()
{
  return parentItem;
}

void FoodTreeItem::removeChild(FoodTreeItem* item)
{
  qDebug() << "Removing children of item " << item->getName();
  item->removeAllChildren();
  qDebug() << "Removing item " << item->getName();
  item->remove();
  childItems.removeOne(item); // Won't ever be more than one
  delete item;
}

void FoodTreeItem::removeAllChildren()
{
  for (QList<FoodTreeItem*>::iterator i = childItems.begin(); i != childItems.end(); ++i) {
    qDebug() << "Removing children of item " << (*i)->getName();
    (*i)->removeAllChildren();
    qDebug() << "Removing item " << (*i)->getName();
    (*i)->remove();
    delete *i;
  }
  childItems.clear();
}

FoodContextMenu* FoodTreeItem::getContextMenu()
{
  return new FoodAmountContextMenu(getIndex(), getFoodAmount());
}

FoodTreeItem* FoodTreeItem::addChild(FoodTreeItem* item)
{
  childItems.append(item);
  return item;
}

QModelIndex FoodTreeItem::getIndex() const
{
  if (model) {
    if (parentItem) {
      QModelIndex index = model->index(row(), 0, parentItem->getIndex());
      qDebug() << "Index of " << getName() << " is " << index;
      return index;
    } else {
      qDebug() << getName() << " is the root";
      return QModelIndex();
    }
  } else {
    return QModelIndex();
  }
}
