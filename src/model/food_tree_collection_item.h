/*
 * food_tree_collection_item.h
 *
 *  Created on: May 26, 2010
 *      Author: tyler
 */

#ifndef FOOD_TREE_COLLECTION_ITEM_H_
#define FOOD_TREE_COLLECTION_ITEM_H_

#include "food_tree_item.h"
#include "food_tree_meal_item.h"
#include "food_tree_component_item.h"
#include "data/food_collection.h"

class FoodTreeCollectionItem : public FoodTreeItem
{
  public:

    FoodTreeCollectionItem(const QAbstractItemModel* model,
                             const QSharedPointer<FoodCollection>& collection,
                             FoodTreeItem* parent = NULL);

    virtual ~FoodTreeCollectionItem();

    FoodTreeCollectionItem* addCollection(const QSharedPointer<FoodCollection>& collection);

    FoodTreeMealItem* addMeal(const QSharedPointer<Meal>& meal);

    FoodTreeComponentItem* addComponent(const FoodComponent& component);

  protected:

    virtual FoodAmount getFoodAmount() const;

    virtual QString getName() const;

    virtual bool showAmount() const { return false; }

  private:

    QSharedPointer<FoodCollection> collection;
};

#endif /* FOOD_TREE_COLLECTION_ITEM_H_ */
