#ifndef FOOD_TREE_H
#define FOOD_TREE_H

#include <QtGui/QWidget>
#include <QtGui/QTreeView>
#include <QScopedPointer>
#include <QModelIndex>
#include <QString>
#include <QVector>
#include <QDate>
#include "libnutrition/data/meal.h"
#include "libnutrition/data/food_collection.h"
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

    // Currently will not take effect until after next clear()
    void setRootCollection(const QSharedPointer<FoodCollection>& collection);

    QSharedPointer<const FoodCollection> getAllFoods() const;

    QVector<QSharedPointer<const Meal> > getAllMeals() const;

  signals:

     void contentsModified();

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

    void changeUnit(const QModelIndex& index, FoodComponent* component,
                      const QSharedPointer<const Unit>& unit);

    void copyMealToDay(const QModelIndex& index, const QSharedPointer<const Meal>& meal,
                         const QDate& date);

  private:

    Ui::FoodTreeUI ui;

    bool temporary;
    QString rootName;
    QDate date;
    QSharedPointer<FoodCollection> rootCollection;

    QScopedPointer<FoodTreeModel> model;
};

#endif // FOOD_TREE_H
