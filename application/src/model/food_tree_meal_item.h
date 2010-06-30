/*
 * food_tree_meal_item.h
 *
 *  Created on: May 26, 2010
 *      Author: tyler
 */

#ifndef FOOD_TREE_MEAL_ITEM_H_
#define FOOD_TREE_MEAL_ITEM_H_

#include "food_tree_item.h"
#include "food_tree_component_item.h"
#include "data/meal.h"

class FoodTreeMealItem : public FoodTreeItem
{
  public:

    FoodTreeMealItem(const QAbstractItemModel* model,
                       const QSharedPointer<Meal>& meal,
                       FoodTreeItem* parent = NULL);

    virtual ~FoodTreeMealItem();

    virtual FoodContextMenu* getContextMenu();

    FoodTreeComponentItem* addComponent(const FoodComponent& component);

    FoodTreeComponentItem* getComponentItem(const FoodComponent& component) const;

    void removeComponent(const FoodComponent& component);

    virtual bool isMeal() const { return true; }

    inline void saveMealToDatabase() { meal->saveToDatabase(); }

  protected:

    virtual FoodAmount getFoodAmount() const;

    virtual QString getName() const;

    virtual bool showAmount() const { return false; }

  private:

    QSharedPointer<Meal> meal;

    QMap<int, FoodTreeComponentItem*> componentChildren;

};

#endif /* FOOD_TREE_MEAL_ITEM_H_ */
