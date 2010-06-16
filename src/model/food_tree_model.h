/*
 * food_tree_model.h
 *
 *  Created on: May 21, 2010
 *      Author: tyler
 */

#ifndef FOOD_TREE_MODEL_H_
#define FOOD_TREE_MODEL_H_

#include "food_tree_item.h"
#include "food_tree_collection_item.h"
#include "food_tree_meal_item.h"
#include "food_context_menu.h"
#include "data/meal.h"
#include "data/food_collection.h"
#include <QAbstractItemModel>
#include <QDate>
#include <QtGui/QTreeView>

class FoodTreeModel : public QAbstractItemModel
{
    Q_OBJECT;

  public:

    explicit FoodTreeModel(QTreeView *treeView,
                              const QString& allFoodsTitle = "All Foods",
                              bool temporaryMeals = true);

    FoodTreeModel(QTreeView *treeView, const QDate& mealsDate,
                    const QString& allFoodsTitle = "All Foods",
                    bool temporaryMeals = true);

    FoodTreeModel(QTreeView *treeView, const QDate& mealsDate,
                    const QSharedPointer<FoodCollection>& rootCollection,
                    bool temporaryCollection = true);

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

    QModelIndex getRootCollectionIndex() const;

    inline QSharedPointer<const FoodCollection> getAllFoods() const
      { return allFoods; }

    QVector<QSharedPointer<const Meal> > getAllMeals() const;

    FoodContextMenu* getContextMenu(const QModelIndex& index) const;

    void removeComponent(FoodComponent& component);

  public slots:

    void addFoodAmount(const FoodAmount& foodAmount);

    void addFoodAmount(const FoodAmount& foodAmount, int mealId);

    void addMeal(const QSharedPointer<const Meal>& meal);

    void changeAmount(const QModelIndex& index, const FoodAmount& newAmount);

  signals:

    void newGroupingCreated(const QModelIndex& index);

  private:

    QTreeView* treeView;

    FoodTreeCollectionItem* rootItem;

    QSharedPointer<FoodCollection> allFoods;
    FoodTreeCollectionItem* allFoodsRoot;

    bool temporaryMeals;
    QDate mealsDate;
    QMap<int, FoodTreeMealItem*> mealRoots;
    QMap<int, QSharedPointer<Meal> > meals;

    void ensureMealRootExists(int mealId);

    void removeAllChildren(const QModelIndex& index);
};

#endif /* FOOD_TREE_MODEL_H_ */
