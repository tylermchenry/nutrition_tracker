/*
 * food_tree_model.h
 *
 *  Created on: May 21, 2010
 *      Author: tyler
 */

#ifndef FOOD_TREE_MODEL_H_
#define FOOD_TREE_MODEL_H_

#include "food_tree_item.h"
#include "data/meal.h"
#include "data/food_collection.h"
#include <QAbstractItemModel>
#include <QtGui/QTreeView>

class FoodTreeModel : public QAbstractItemModel
{
    Q_OBJECT;

  public:

    explicit FoodTreeModel(QTreeView *parent);
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

    void addFoodAmount(const FoodAmount& foodAmount, int mealId);

  signals:

    void newGroupingCreated(const QModelIndex& index);

  private:

    QTreeView* treeView;

    FoodTreeItem* rootItem;

    QSharedPointer<FoodCollection> proposedAdditions;
    FoodTreeItem* proposedAdditionsRoot;

    QMap<int, FoodTreeItem*> mealRoots;
    QMap<int, QSharedPointer<Meal> > temporaryMeals;

};

#endif /* FOOD_TREE_MODEL_H_ */
