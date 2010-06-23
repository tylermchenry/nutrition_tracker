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
    QList<FoodComponent> components = collection->getComponents();
    for (QList<FoodComponent>::const_iterator i = components.begin(); i != components.end(); ++i) {
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
  collectionChildren[collection->getId()] = item;
  return item;
}

FoodTreeMealItem* FoodTreeCollectionItem::addMeal(const QSharedPointer<Meal>& meal)
{
  FoodTreeMealItem* item = new FoodTreeMealItem(getModel(), meal, this);
  addChild(item);
  mealChildren[meal->getId()] = item;
  return item;
}

FoodTreeComponentItem* FoodTreeCollectionItem::addComponent(const FoodComponent& component)
{
  FoodTreeComponentItem* item = new FoodTreeComponentItem(getModel(), component, this);
  addChild(item);
  componentChildren[component.getId()] = item;
  return item;
}

FoodTreeComponentItem* FoodTreeCollectionItem::getComponentItem(const FoodComponent& component) const
{
  if (componentChildren.contains(component.getId())) {
    return componentChildren[component.getId()];
  } else {
    return NULL;
  }
}

void FoodTreeCollectionItem::removeCollection(const QSharedPointer<FoodCollection>& collection)
{
  if (collectionChildren.contains(collection->getId())) {
    delete collectionChildren[collection->getId()];
    collectionChildren.remove(collection->getId());
  }
}

void FoodTreeCollectionItem::removeMeal(const QSharedPointer<Meal>& meal)
{
  if (mealChildren.contains(meal->getId())) {
    delete mealChildren[meal->getId()];
    mealChildren.remove(meal->getId());
  }
}

void FoodTreeCollectionItem::removeComponent(const FoodComponent& component)
{
  if (componentChildren.contains(component.getId())) {
    delete componentChildren[component.getId()];
    componentChildren.remove(component.getId());
  }
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
    return collection->getDisplayName();
  } else {
    return "(Undefined Collection)";
  }
}
