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
#include <QDate>
#include <QtGui/QTreeView>
#include "data/food_collection.h"

class FoodTreeModel : public QAbstractItemModel
{
    Q_OBJECT;

  public:

    explicit FoodTreeModel(QTreeView *parent, const QString& allFoodsTitle = "All Foods");

    FoodTreeModel(QTreeView *parent, const QDate& mealsDate,
                    const QString& allFoodsTitle = "All Foods");

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

    inline QSharedPointer<const FoodCollection> getAllFoods() const
      { return allFoods; }

    QVector<QSharedPointer<const Meal> > getAllMeals() const;

  public slots:

    void addFoodAmount(const FoodAmount& foodAmount, int mealId);

    void addMeal(const QSharedPointer<const Meal>& meal);

  signals:

    void newGroupingCreated(const QModelIndex& index);

  private:

    QTreeView* treeView;

    FoodTreeItem* rootItem;

    QSharedPointer<FoodCollection> allFoods;
    FoodTreeItem* allFoodsRoot;

    QDate mealsDate;
    QMap<int, FoodTreeItem*> mealRoots;
    QMap<int, QSharedPointer<Meal> > temporaryMeals;

    void ensureMealRootExists(int mealId);
};

#endif /* FOOD_TREE_MODEL_H_ */
