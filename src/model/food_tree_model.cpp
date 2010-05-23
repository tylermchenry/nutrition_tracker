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

FoodTreeModel::FoodTreeModel(QTreeView *treeView, const QString& allFoodsTitle)
  : QAbstractItemModel(treeView), treeView(treeView),
    rootItem(new FoodTreeItem(FoodAmount())),
    allFoods(FoodCollection::createFoodCollection(allFoodsTitle))
{
  beginInsertRows(QModelIndex(), rootItem->childCount(), rootItem->childCount());
  allFoodsRoot = rootItem->addChild(allFoods->getBaseAmount());
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

QVector<QSharedPointer<const Meal> > FoodTreeModel::getAllMeals() const
{
  QVector<QSharedPointer<const Meal> > constMeals;
  QList<QSharedPointer<Meal> > meals = temporaryMeals.values();

  // TODO: There has to be a more elegant way to convert from a collection
  // of T to a collection of const T

  for (QList<QSharedPointer<Meal> >::const_iterator i = meals.begin(); i != meals.end(); ++i) {
    constMeals.push_back(*i);
  }

  return constMeals;
}

void FoodTreeModel::addFoodAmount(const FoodAmount& foodAmount, int mealId)
{
  qDebug() << "Adding food amount to food tree model";

  ensureMealRootExists(mealId);

  FoodTreeItem* parentOfNewItem = mealRoots[mealId];

  temporaryMeals[mealId]->addComponent(foodAmount);
  beginInsertRows(createIndex(parentOfNewItem->row(), 0, parentOfNewItem),
                  parentOfNewItem->childCount(), parentOfNewItem->childCount());
  parentOfNewItem->addChild(foodAmount);
  endInsertRows();
}

void FoodTreeModel::addMeal(const QSharedPointer<const Meal>& meal)
{
  int mealId = meal->getMealId();

  ensureMealRootExists(mealId);

  FoodTreeItem* parentOfNewItem = mealRoots[mealId];

  temporaryMeals[mealId]->mergeMeal(meal);

  QVector<FoodAmount> components = meal->getComponents();

  beginInsertRows(createIndex(parentOfNewItem->row(), 0, parentOfNewItem),
                    parentOfNewItem->childCount(), parentOfNewItem->childCount()+components.size()-1);

  for (QVector<FoodAmount>::const_iterator i = components.begin();
      i != components.end(); ++i)
  {
    parentOfNewItem->addChild(*i);
  }

  endInsertRows();
}

void FoodTreeModel::ensureMealRootExists(int mealId)
{
  if (allFoodsRoot->childCount() == 0) {
     emit newGroupingCreated(createIndex(allFoodsRoot->row(), 0, allFoodsRoot));
   }

   if (!mealRoots.contains(mealId)) {
     QSharedPointer<Meal> newMeal = Meal::createTemporaryMeal(1, QDate::currentDate(), mealId);
     temporaryMeals[mealId] = newMeal;
     allFoods->addComponent(newMeal->getBaseAmount());
     beginInsertRows(createIndex(allFoodsRoot->row(), 0, allFoodsRoot),
                       allFoodsRoot->childCount(),
                       allFoodsRoot->childCount());
     mealRoots[mealId] = allFoodsRoot->addChild(newMeal->getBaseAmount());
     endInsertRows();
     emit newGroupingCreated(createIndex(mealRoots[mealId]->row(), 0, mealRoots[mealId]));
   }
}
