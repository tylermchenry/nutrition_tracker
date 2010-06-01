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

    explicit FoodTreeItem(const QAbstractItemModel* model, FoodTreeItem* parentItem = NULL);

    virtual ~FoodTreeItem();

    FoodTreeItem* child(int row);

    int childCount() const;

    int columnCount() const;

    QVariant data(int column) const;

    int row() const;

    int depth() const;

    FoodTreeItem *parent();

    void removeChild(FoodTreeItem* item);

    void removeAllChildren();

    virtual FoodContextMenu* getContextMenu();

    virtual bool isMeal() const { return false; }

  protected:

    virtual FoodAmount getFoodAmount() const = 0;

    virtual QString getName() const = 0;

    virtual bool showName() const { return true; }

    virtual bool showAmount() const { return true; }

    virtual bool showNutrients() const { return true; }

    virtual void remove() {};

    FoodTreeItem* addChild(FoodTreeItem* item);

    inline const QAbstractItemModel* getModel() const { return model; }

    QModelIndex getIndex() const;

  private:

    const QAbstractItemModel* model;

    QVector<QString> columnNutrientIds;
    QList<FoodTreeItem*> childItems;
    FoodTreeItem* parentItem;
};

#endif /* FOOD_TREE_ITEM_H_ */
