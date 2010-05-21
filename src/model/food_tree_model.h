/*
 * food_tree_model.h
 *
 *  Created on: May 21, 2010
 *      Author: tyler
 */

#ifndef FOOD_TREE_MODEL_H_
#define FOOD_TREE_MODEL_H_

#include "food_tree_item.h"
#include <QAbstractItemModel>

class FoodTreeModel : public QAbstractItemModel
{
    Q_OBJECT;

  public:

    explicit FoodTreeModel(QObject *parent = 0);
    ~FoodTreeModel();

    QVariant data(const QModelIndex &index, int role) const;

    Qt::ItemFlags flags(const QModelIndex &index) const;

    QVariant headerData(int section, Qt::Orientation orientation,
                        int role = Qt::DisplayRole) const;

    QModelIndex index(int row, int column,
                      const QModelIndex &parent = QModelIndex()) const;

    QModelIndex parent(const QModelIndex &index) const;

    int rowCount(const QModelIndex &parent = QModelIndex()) const;

    int columnCount(const QModelIndex &parent = QModelIndex()) const;

  public slots:

    void addFoodAmount(const FoodAmount& foodAmount);

  private:

    FoodTreeItem *rootItem;

};

#endif /* FOOD_TREE_MODEL_H_ */
