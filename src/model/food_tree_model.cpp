/*
 * food_tree_model.cpp
 *
 *  Created on: May 21, 2010
 *      Author: tyler
 */

#include "food_tree_model.h"
#include <QDebug>
#include <QtGui/QFont>
#include <QtGui/QTreeView>

FoodTreeModel::FoodTreeModel(QTreeView *treeView)
  : QAbstractItemModel(treeView), treeView(treeView),
    rootItem(new FoodTreeItem(FoodAmount())),
    proposedAdditions(FoodCollection::createFoodCollection("Proposed Additions"))
{
  beginInsertRows(QModelIndex(), rootItem->childCount(), rootItem->childCount());
  proposedAdditionsRoot = rootItem->addChild(proposedAdditions->getBaseAmount());
  endInsertRows();
}

FoodTreeModel::~FoodTreeModel()
{
  delete rootItem;
}

QVariant FoodTreeModel::data(const QModelIndex &index, int role) const
{
  if (!index.isValid())
    return QVariant();

  FoodTreeItem *item = static_cast<FoodTreeItem*>(index.internalPointer());

  if (role == Qt::DisplayRole) {

    // Hide amounts for depths of 1 and 2 (summaries)
    if (index.column() != 1 || item->depth() >= 3) {
      return item->data(index.column());
    }

  } else if (role == Qt::FontRole) {

    QFont font;
    int depth = item->depth();

    if (depth < 2) {
      font.setItalic(true);
    }

    if (depth < 3) {
      font.setBold(true);
    } else if (item->childCount() > 0) {
      if (treeView && treeView->isExpanded(createIndex(index.row(), 0, index.internalPointer()))) {
        font.setItalic(true);
      }
    }

    return font;

  } else if (role == Qt::TextAlignmentRole) {

    if (index.column() == 0) {
      return Qt::AlignLeft;
    } else {
      return Qt::AlignRight;
    }
  }

  return QVariant();
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
      return "Carbs";
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
    return static_cast<FoodTreeItem*>(parent.internalPointer())->columnCount() + 1;
  else
    return rootItem->columnCount() + 1;
}

void FoodTreeModel::addFoodAmount(const FoodAmount& foodAmount, int mealId)
{
  qDebug() << "Adding food amount to food tree model";

  if (proposedAdditionsRoot->childCount() == 0) {
    emit newGroupingCreated(createIndex(proposedAdditionsRoot->row(), 0, proposedAdditionsRoot));
  }

  if (!mealRoots.contains(mealId)) {
    QSharedPointer<Meal> newMeal = Meal::createTemporaryMeal(1, QDate::currentDate(), mealId);
    temporaryMeals[mealId] = newMeal;
    proposedAdditions->addComponent(newMeal->getBaseAmount());
    beginInsertRows(createIndex(proposedAdditionsRoot->row(), 0, proposedAdditionsRoot),
                      proposedAdditionsRoot->childCount(),
                      proposedAdditionsRoot->childCount());
    mealRoots[mealId] = proposedAdditionsRoot->addChild(newMeal->getBaseAmount());
    endInsertRows();
    emit newGroupingCreated(createIndex(mealRoots[mealId]->row(), 0, mealRoots[mealId]));
  }

  FoodTreeItem* parentOfNewItem = mealRoots[mealId];

  temporaryMeals[mealId]->addComponent(foodAmount);
  beginInsertRows(createIndex(parentOfNewItem->row(), 0, parentOfNewItem),
                  parentOfNewItem->childCount(), parentOfNewItem->childCount());
  parentOfNewItem->addChild(foodAmount);
  endInsertRows();
}
