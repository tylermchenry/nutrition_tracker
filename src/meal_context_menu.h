/*
 * meal_context_menu.h
 * Part of nutrition_tracker
 *
 *  Created on: Jun 6, 2010
 *      Author: Tyler McHenry <tyler@nerdland.net>
 */

#ifndef MEAL_CONTEXT_MENU_H_
#define MEAL_CONTEXT_MENU_H_

#include "food_context_menu.h"
#include <QModelIndex>
#include "data/meal.h"

class MealContextMenu : public FoodContextMenu
{
  public:

    MealContextMenu(const QModelIndex& index, const QSharedPointer<Meal>& meal, QWidget* parent = 0);

    virtual ~MealContextMenu();

  private slots:

    virtual void actionTriggered(QAction* action);

  private:

    QSharedPointer<Meal> meal;

    QAction* actViewNutritionInfo;
    QMap<QAction*, QDate> actCopyToDay;

    QMenu* mnuCopyToDay;
};

#endif /* MEAL_CONTEXT_MENU_H_ */
