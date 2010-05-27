/*
 * food_tree_collection_item.cpp
 *
 *  Created on: May 26, 2010
 *      Author: tyler
 */

#include "food_tree_collection_item.h"

FoodTreeCollectionItem::FoodTreeCollectionItem
  (const QAbstractItemModel* model, const QSharedPointer<FoodCollection>& collection,
   FoodTreeItem* parent)
  : FoodTreeItem(model, parent), collection(collection)
{
  if (collection) {
    QSet<FoodComponent> components = collection->getComponents();
    for (QSet<FoodComponent>::const_iterator i = components.begin(); i != components.end(); ++i) {
      addComponent(*i);
    }
  }
}

FoodTreeCollectionItem::~FoodTreeCollectionItem()
{
}

FoodTreeCollectionItem* FoodTreeCollectionItem::addCollection(const QSharedPointer<FoodCollection>& collection)
{
  FoodTreeCollectionItem* item = new FoodTreeCollectionItem(getModel(), collection, this);
  addChild(item);
  return item;
}

FoodTreeMealItem* FoodTreeCollectionItem::addMeal(const QSharedPointer<Meal>& meal)
{
  FoodTreeMealItem* item = new FoodTreeMealItem(getModel(), meal, this);
  addChild(item);
  return item;
}

FoodTreeComponentItem* FoodTreeCollectionItem::addComponent(const FoodComponent& component)
{
  FoodTreeComponentItem* item = new FoodTreeComponentItem(getModel(), component, this);
  addChild(item);
  return item;
}

FoodAmount FoodTreeCollectionItem::getFoodAmount() const
{
  if (collection) {
    return collection->getBaseAmount();
  } else {
    return FoodAmount();
  }
}

QString FoodTreeCollectionItem::getName() const
{
  if (collection) {
    return collection->getName();
  } else {
    return "(Undefined Collection)";
  }
}
