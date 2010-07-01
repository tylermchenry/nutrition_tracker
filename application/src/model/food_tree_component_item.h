/*
 * food_tree_component_item.h
 *
 *  Created on: May 26, 2010
 *      Author: tyler
 */

#ifndef FOOD_TREE_COMPONENT_ITEM_H_
#define FOOD_TREE_COMPONENT_ITEM_H_

#include "food_tree_item.h"
#include "libnutrition/data/food_component.h"

class FoodTreeComponentItem : public FoodTreeItem
{
  public:

    FoodTreeComponentItem(const QAbstractItemModel* model,
                            const FoodComponent& component,
                            FoodTreeItem* parent = NULL);

    virtual ~FoodTreeComponentItem();

    virtual FoodContextMenu* getContextMenu();

    virtual void changeAmount(const FoodAmount& amount);

  protected:

    virtual FoodAmount getFoodAmount() const;

    virtual QString getName() const;

    virtual void remove();

  private:

    FoodComponent component;
};

#endif /* FOOD_TREE_COMPONENT_ITEM_H_ */
