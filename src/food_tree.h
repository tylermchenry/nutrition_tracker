#ifndef FOOD_TREE_H
#define FOOD_TREE_H

#include <QtGui/QWidget>
#include <QtGui/QTreeView>
#include <QModelIndex>
#include <QString>
#include <QVector>
#include <QDate>
#include "data/meal.h"
#include "data/food_collection.h"
#include "model/food_tree_model.h"
#include "ui_food_tree.h"

class FoodTree : public QWidget
{
    Q_OBJECT

  public:

    explicit FoodTree(QWidget *parent = 0);

    FoodTree(const QString& rootName, const QDate& date, QWidget *parent = 0);

    ~FoodTree();

    void initialize();

    // Currently will not take effect until after next clear()
    void setTemporary(bool temporary);

    // Currently will not take effect until after next clear()
    void setRootName(const QString& newRootName);

    // Currently will not take effect until after next clear()
    void setDate(const QDate& newDate);

    QSharedPointer<const FoodCollection> getAllFoods() const;

    QVector<QSharedPointer<const Meal> > getAllMeals() const;

  signals:

    void mealModified(const QSharedPointer<Meal>& meal);

  public slots:

    void clear();

    void addFoodAmount(const FoodAmount& foodAmount);

    void addFoodAmount(const FoodAmount& foodAmount, int mealId);

    void addMeal(const QSharedPointer<const Meal>& meal);

    void addMeals(const QVector<QSharedPointer<const Meal> >& meals);

    void addMeals(const QVector<QSharedPointer<Meal> >& meals);

  private slots:

    void expandGrouping(const QModelIndex& index);

    void showContextMenu(const QPoint& point);

    void removeComponent(const QModelIndex& index, FoodComponent* component);

    void displayNutritionInfo(const QModelIndex& index, const FoodAmount& amount);

    void changeAmount(const QModelIndex& index, FoodComponent* component);

  private:

    Ui::FoodTreeUI ui;

    bool temporary;
    QString rootName;
    QDate date;

    FoodTreeModel* model;
};

#endif // FOOD_TREE_H
