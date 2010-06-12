/*
 * food_context_menu.h
 *
 *  Created on: May 26, 2010
 *      Author: tyler
 */

#ifndef FOOD_CONTEXT_MENU_H_
#define FOOD_CONTEXT_MENU_H_

#include <QtGui/QMenu>
#include <QModelIndex>
#include "data/food_amount.h"
#include "data/food_component.h"
#include "data/meal.h"

class FoodContextMenu : public QMenu
{
    Q_OBJECT;

  public:

    virtual ~FoodContextMenu();

  signals:

    void viewNutritionInformation(const QModelIndex& index, const FoodAmount& foodAmount);

    void changeAmount(const QModelIndex& index, FoodComponent* component);
    void changeUnit(const QModelIndex& index, FoodComponent* component, const QSharedPointer<const Unit>& unit);
    void edit(const QModelIndex& index, FoodComponent* component);
    void duplicate(const QModelIndex& index, FoodComponent* component);
    void moveToMeal(const QModelIndex& index, FoodComponent* component, int mealId);
    void remove(const QModelIndex& index, FoodComponent* component);

    void copyMealToDay(const QModelIndex& index, const QSharedPointer<const Meal>& meal, const QDate& date);

  protected:

    FoodContextMenu(const QModelIndex& index, QWidget* parent = 0);

    QModelIndex getIndex() const { return index; }

  protected slots:

    virtual void actionTriggered(QAction* action) = 0;

  private:

    QModelIndex index;
};

#endif /* FOOD_CONTEXT_MENU_H_ */
