/*
 * food_tree_item.h
 *
 *  Created on: May 21, 2010
 *      Author: tyler
 */

#ifndef FOOD_TREE_ITEM_H_
#define FOOD_TREE_ITEM_H_

#include "data/food_amount.h"
#include <QList>

class FoodTreeItem
{
  public:

    FoodTreeItem(const FoodAmount& foodAmount, FoodTreeItem* parentItem = NULL);

    virtual ~FoodTreeItem();

    void addChild(const FoodAmount& foodAmount);

    FoodTreeItem* child(int row);

    int childCount() const;

    int columnCount() const;

    QVariant data(int column) const;

    int row() const;

    FoodTreeItem *parent();

  private:

    FoodAmount foodAmount;
    QList<FoodTreeItem*> childItems;
    FoodTreeItem* parentItem;

};

#endif /* FOOD_TREE_ITEM_H_ */
