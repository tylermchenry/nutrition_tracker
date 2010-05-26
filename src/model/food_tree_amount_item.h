/*
 * food_tree_amount_item.h
 *
 *  Created on: May 26, 2010
 *      Author: tyler
 */

#ifndef FOOD_TREE_AMOUNT_ITEM_H_
#define FOOD_TREE_AMOUNT_ITEM_H_

#include "food_tree_item.h"

class FoodTreeAmountItem : public FoodTreeItem
{
  public:

    explicit FoodTreeAmountItem(const FoodAmount& foodAmount, FoodTreeItem* parent = NULL);

    virtual ~FoodTreeAmountItem();

  protected:

     virtual FoodAmount getFoodAmount() const;

     virtual QString getName() const;

  private:

     FoodAmount foodAmount;
};

#endif /* FOOD_TREE_AMOUNT_ITEM_H_ */
