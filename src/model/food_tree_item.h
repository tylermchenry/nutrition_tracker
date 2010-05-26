/*
 * food_tree_item.h
 *
 *  Created on: May 21, 2010
 *      Author: tyler
 */

#ifndef FOOD_TREE_ITEM_H_
#define FOOD_TREE_ITEM_H_

#include "food_context_menu.h"
#include "data/food_amount.h"
#include <QList>
#include <QtGui/QMenu>

class FoodTreeItem
{
  public:

    explicit FoodTreeItem(FoodTreeItem* parentItem = NULL);

    virtual ~FoodTreeItem();

    FoodTreeItem* child(int row);

    int childCount() const;

    int columnCount() const;

    QVariant data(int column) const;

    int row() const;

    int depth() const;

    FoodTreeItem *parent();

    virtual FoodContextMenu* getContextMenu() { return NULL; }

  protected:

    virtual FoodAmount getFoodAmount() const = 0;

    virtual QString getName() const = 0;

    virtual bool showName() const { return true; }

    virtual bool showAmount() const { return true; }

    virtual bool showNutrients() const { return true; }

    FoodTreeItem* addChild(FoodTreeItem* item);

  private:

    QVector<QString> columnNutrientIds;
    QList<FoodTreeItem*> childItems;
    FoodTreeItem* parentItem;
};

#endif /* FOOD_TREE_ITEM_H_ */
