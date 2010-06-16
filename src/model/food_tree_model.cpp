/*
 * food_tree_model.cpp
 *
 *  Created on: May 21, 2010
 *      Author: tyler
 */

#include "food_tree_model.h"
#include "food_tree_amount_item.h"
#include <QDebug>
#include <QtGui/QFont>
#include <QtGui/QTreeView>

FoodTreeModel::FoodTreeModel(QTreeView *treeView, const QString& allFoodsTitle,
                                 bool temporaryMeals)
  : QAbstractItemModel(treeView), treeView(treeView),
    rootItem(new FoodTreeCollectionItem(this, QSharedPointer<FoodCollection>())),
    allFoods(FoodCollection::createFoodCollection(allFoodsTitle)),
    temporaryMeals(temporaryMeals), mealsDate(QDate::currentDate())
{
  beginInsertRows(QModelIndex(), rootItem->childCount(), rootItem->childCount());
  allFoodsRoot = rootItem->addCollection(allFoods);
  endInsertRows();
}

FoodTreeModel::FoodTreeModel(QTreeView *treeView, const QDate& mealsDate,
                                 const QString& allFoodsTitle, bool temporaryMeals)
  : QAbstractItemModel(treeView), treeView(treeView),
    rootItem(new FoodTreeCollectionItem(this, QSharedPointer<FoodCollection>())),
    allFoods(FoodCollection::createFoodCollection(allFoodsTitle)),
    temporaryMeals(temporaryMeals), mealsDate(mealsDate)
{
  beginInsertRows(QModelIndex(), rootItem->childCount(), rootItem->childCount());
  allFoodsRoot = rootItem->addCollection(allFoods);
  endInsertRows();
}

FoodTreeModel::FoodTreeModel
  (QTreeView *treeView, const QDate& mealsDate,
   const QSharedPointer<FoodCollection>& rootCollection,
   bool temporaryCollection)
  : QAbstractItemModel(treeView), treeView(treeView),
    rootItem(new FoodTreeCollectionItem(this, QSharedPointer<FoodCollection>())),
    allFoods(temporaryCollection ? FoodCollection::createFoodCollection(rootCollection): rootCollection),
    temporaryMeals(temporaryCollection), mealsDate(mealsDate)
{
  beginInsertRows(QModelIndex(), rootItem->childCount(), rootItem->childCount());
  allFoodsRoot = rootItem->addCollection(allFoods);
  endInsertRows();

  emit newGroupingCreated(createIndex(allFoodsRoot->row(), 0, allFoodsRoot));
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

    return item->data(index.column());

  } else if (role == Qt::FontRole) {

    QFont font;
    int depth = item->depth();

    if (depth < 2) {
      font.setItalic(true);
    }

    if (depth < 2 || item->isMeal()) {
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
  if (!hasIndex(row, column, parent)) {
    qDebug() << "hasIndex() returned false";
    return QModelIndex();
  }

  FoodTreeItem *parentItem;

  if (!parent.isValid())
    parentItem = rootItem;
  else
    parentItem = static_cast<FoodTreeItem*>(parent.internalPointer());

  FoodTreeItem *childItem = parentItem->child(row);
  if (childItem)
    return createIndex(row, column, childItem);
  else {
    qDebug() << "child() returned NULL";
    return QModelIndex();
  }

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

QModelIndex FoodTreeModel::getRootCollectionIndex() const
{
  return createIndex(allFoodsRoot->row(), 0, allFoodsRoot);
}

QVector<QSharedPointer<const Meal> > FoodTreeModel::getAllMeals() const
{
  QVector<QSharedPointer<const Meal> > constMeals;
  QList<QSharedPointer<Meal> > allMeals = meals.values();

  // TODO: There has to be a more elegant way to convert from a collection
  // of T to a collection of const T

  for (QList<QSharedPointer<Meal> >::const_iterator i = allMeals.begin(); i != allMeals.end(); ++i) {
    constMeals.push_back(*i);
  }

  return constMeals;
}

FoodContextMenu* FoodTreeModel::getContextMenu(const QModelIndex& index) const
{
  FoodTreeItem *item = static_cast<FoodTreeItem*>(index.internalPointer());

  if (item) {
    return item->getContextMenu();
  } else {
    return NULL;
  }
}

void FoodTreeModel::removeComponent(FoodComponent& component)
{
  qDebug() << "Attempting to remove component "
           << component.getFoodAmount().getFood()->getName()
           << " from " << component.getContainingCollection()->getName();

  QSharedPointer<Meal> meal = component.getContainingCollection().dynamicCast<Meal>();

  if (meal) {

    FoodTreeMealItem* parentItem = mealRoots[meal->getMealId()];
    FoodTreeComponentItem* item = parentItem->getComponentItem(component);

    if (item) {

      // First, remove all children items of the component item, which will
      // all be amount items, not component items, since the tree widget can't
      // manipulate anything at a lower level than meal components
      removeAllChildren(createIndex(item->row(), 0, item));

      // Actually remove the component from its containing meal, and save
      // to the database if necessary
      meal->removeComponent(component);
      if (!temporaryMeals) meal->saveToDatabase();

      // Finally, remove the item itself from its parent item
      beginRemoveRows(createIndex(parentItem->row(), 0, parentItem),
                      item->row(), item->row());
      parentItem->removeChild(item);
      endRemoveRows();
    } else {
      qDebug() << "Corresponding item not found";
    }

  } else if (component.getContainingCollection()->getId() == allFoods->getId()) {

    FoodTreeComponentItem* item = allFoodsRoot->getComponentItem(component);

    if (item) {
      removeAllChildren(createIndex(item->row(), 0, item));
      allFoods->removeComponent(component);

      beginRemoveRows(createIndex(allFoodsRoot->row(), 0, allFoodsRoot),
                      item->row(), item->row());
      allFoodsRoot->removeChild(item);
      endRemoveRows();
    } else {
      qDebug() << "Corresponding item not found";
    }

  } else {
    qDebug() << "Corresponding meal not found";
  }
}

void FoodTreeModel::addFoodAmount(const FoodAmount& foodAmount)
{
  qDebug() << "Adding food amount to food tree model (no meal)";

  if (allFoodsRoot->childCount() == 0) {
    emit newGroupingCreated(createIndex(allFoodsRoot->row(), 0, allFoodsRoot));
  }

  beginInsertRows(createIndex(allFoodsRoot->row(), 0, allFoodsRoot),
                    allFoodsRoot->childCount(),
                    allFoodsRoot->childCount());

  allFoodsRoot->addComponent(allFoods->addComponent(foodAmount));

  endInsertRows();
}

void FoodTreeModel::addFoodAmount(const FoodAmount& foodAmount, int mealId)
{
  qDebug() << "Adding food amount to food tree model";

  ensureMealRootExists(mealId);

  FoodTreeMealItem* parentOfNewItem = mealRoots[mealId];

  FoodComponent component = meals[mealId]->addComponent(foodAmount);
  if (!temporaryMeals) meals[mealId]->saveToDatabase();

  beginInsertRows(createIndex(parentOfNewItem->row(), 0, parentOfNewItem),
                  parentOfNewItem->childCount(), parentOfNewItem->childCount());

  parentOfNewItem->addComponent(component);
  endInsertRows();
}

void FoodTreeModel::addMeal(const QSharedPointer<const Meal>& meal)
{
  int mealId = meal->getMealId();

  qDebug() << "Model adding " << (meal->isTemporary() ? "temporary" : "")
           << " meal: " << meal->getName() << " for " << meal->getDate().toString();


  ensureMealRootExists(mealId);

  FoodTreeMealItem* parentOfNewItem = mealRoots[mealId];

  if (!temporaryMeals && !meal->isTemporary()) {
    // In this case, the model is DB-backed and the meal passed in is DB-backed
    // so we assume we just loaded the same meal out of the DB with the
    // ensureMealRoot call, and so merging the loaded and the passed-in meal
    // would duplicate things, as would adding additional items below
    // the meal's item that was created in ensureMealRoot.
    return;
  }

  qDebug() << "Model adding " << (meal->isTemporary() ? "temporary" : "")
            << " meal: " << meal->getName() << " for " << meal->getDate().toString()
            << " into " << (meals[mealId]->isTemporary() ? "temporary" : "")
            << " meal: " << meals[mealId]->getName() << " for " << meals[mealId]->getDate().toString();

  QVector<FoodComponent> newComponents = meals[mealId]->merge(meal);

  if (!temporaryMeals) {
    qDebug() << "Model saving  meal: " << meal->getName() << " for "
              << meal->getDate().toString() << " to DB.";
    meals[mealId]->saveToDatabase();
  }

  qDebug() << "Meal has " << newComponents.size() << " new components.";

  beginInsertRows(createIndex(parentOfNewItem->row(), 0, parentOfNewItem),
                    parentOfNewItem->childCount(), parentOfNewItem->childCount()+newComponents.size()-1);

  for (QVector<FoodComponent>::const_iterator i = newComponents.begin();
      i != newComponents.end(); ++i)
  {
    parentOfNewItem->addComponent(*i);
  }

  endInsertRows();
}

void FoodTreeModel::changeAmount(const QModelIndex& index, const FoodAmount& newAmount)
{
  // TODO: Try to to this with less casting

  if (hasIndex(index.row(), index.column(), index.parent())) {
    FoodTreeItem* item = static_cast<FoodTreeItem*>(index.internalPointer());
    FoodTreeComponentItem* componentItem;

    if ((componentItem = dynamic_cast<FoodTreeComponentItem*>(item))) {
      componentItem->changeAmount(newAmount);
    }

    for (QModelIndex idx = index; idx != QModelIndex(); idx = idx.parent()) {
      FoodTreeItem* changedItem = static_cast<FoodTreeItem*>(idx.internalPointer());
      if (!temporaryMeals && changedItem->isMeal()) {
        dynamic_cast<FoodTreeMealItem*>(changedItem)->saveMealToDatabase();
      }
      emit dataChanged(idx, idx);
    }
  }
}

void FoodTreeModel::ensureMealRootExists(int mealId)
{
  if (allFoodsRoot->childCount() == 0) {
     emit newGroupingCreated(createIndex(allFoodsRoot->row(), 0, allFoodsRoot));
   }

   if (!mealRoots.contains(mealId)) {

     QSharedPointer<Meal> newMeal =
         temporaryMeals ? Meal::createTemporaryMeal(1, mealsDate, mealId) :
                          Meal::getOrCreateMeal(1, mealsDate, mealId);

     meals[mealId] = newMeal;

     allFoods->addComponent(newMeal->getBaseAmount());

     beginInsertRows(createIndex(allFoodsRoot->row(), 0, allFoodsRoot),
                       allFoodsRoot->childCount(),
                       allFoodsRoot->childCount());
     mealRoots[mealId] = allFoodsRoot->addMeal(newMeal);
     endInsertRows();

     emit newGroupingCreated(createIndex(mealRoots[mealId]->row(), 0, mealRoots[mealId]));
   }
}

void FoodTreeModel::removeAllChildren(const QModelIndex& index)
{
  FoodTreeItem* item = static_cast<FoodTreeItem*>(index.internalPointer());

  qDebug() << "Attempting to remove all children of item at " << index;

  // Recursively remove all child items of the given item (but not the given
  // item itself). This method assumes that these items are not DB-backed.

  if (item->childCount() > 0) {

    qDebug() << "Step 1: Item has " << item->childCount() << " children";

    for (int i = 0; i < item->childCount(); ++i) {
      removeAllChildren(createIndex(i, 0, item->child(i)));
    }

    qDebug() << "Step 2: Item has " << item->childCount() << " children";

    beginRemoveRows(createIndex(item->row(), 0, item),
                      0, item->childCount()-1);
    item->removeAllChildren();
    endRemoveRows();

    qDebug() << "Step 3: Item has ??? children";
  }
}

