/*
 * food_tree_model.cpp
 *
 *  Created on: May 21, 2010
 *      Author: tyler
 */

#include "food_tree_model.h"
#include <QDebug>

FoodTreeModel::FoodTreeModel(QObject *parent)
  : QAbstractItemModel(parent), rootItem(new FoodTreeItem(FoodAmount()))
{
}

FoodTreeModel::~FoodTreeModel()
{
  delete rootItem;
}

QVariant FoodTreeModel::data(const QModelIndex &index, int role) const
{
  if (!index.isValid())
    return QVariant();

  if (role != Qt::DisplayRole)
    return QVariant();

  FoodTreeItem *item = static_cast<FoodTreeItem*>(index.internalPointer());

  return item->data(index.column());
}

Qt::ItemFlags FoodTreeModel::flags(const QModelIndex &index) const
{
  if (!index.isValid())
    return 0;

  return Qt::ItemIsEnabled | Qt::ItemIsSelectable;
}

QVariant FoodTreeModel::headerData(int section, Qt::Orientation orientation, int role) const
{
  if (orientation == Qt::Horizontal && role == Qt::DisplayRole) {
    if (section == 0) {
      return "Food";
    } else if (section == 1) {
      return "Amount";
    } else if (section == 2) {
      return "Calories";
    } else if (section == 3) {
      return "Fat";
    } else if (section == 4) {
      return "Protein";
    } else if (section == 5) {
      return "Carbohydrate";
    }

  }

  return QVariant();
}

QModelIndex FoodTreeModel::index(int row, int column, const QModelIndex &parent) const
{
  if (!hasIndex(row, column, parent))
    return QModelIndex();

  FoodTreeItem *parentItem;

  if (!parent.isValid())
    parentItem = rootItem;
  else
    parentItem = static_cast<FoodTreeItem*>(parent.internalPointer());

  FoodTreeItem *childItem = parentItem->child(row);
  if (childItem)
    return createIndex(row, column, childItem);
  else
    return QModelIndex();
}

QModelIndex FoodTreeModel::parent(const QModelIndex &index) const
{
  if (!index.isValid())
    return QModelIndex();

  FoodTreeItem *childItem = static_cast<FoodTreeItem*>(index.internalPointer());
  FoodTreeItem *parentItem = childItem->parent();

  if (parentItem == rootItem)
    return QModelIndex();

  return createIndex(parentItem->row(), 0, parentItem);
}

int FoodTreeModel::rowCount(const QModelIndex &parent) const
{
  FoodTreeItem *parentItem;
  if (parent.column() > 0)
    return 0;

  if (!parent.isValid())
    parentItem = rootItem;
  else
    parentItem = static_cast<FoodTreeItem*>(parent.internalPointer());

  return parentItem->childCount();
}

int FoodTreeModel::columnCount(const QModelIndex &parent) const
{
  if (parent.isValid())
    return static_cast<FoodTreeItem*>(parent.internalPointer())->columnCount();
  else
    return rootItem->columnCount();
}

void FoodTreeModel::addFoodAmount(const FoodAmount& foodAmount, int mealId)
{
  qDebug() << "Adding food amount to food tree model";

  beginInsertRows(QModelIndex(), rootItem->childCount(), rootItem->childCount());
  rootItem->addChild(foodAmount);
  endInsertRows();
}
