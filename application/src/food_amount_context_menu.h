/*
 * food_amount_context_menu.h
 * Part of nutrition_tracker
 *
 *  Created on: May 31, 2010
 *      Author: Tyler McHenry <tyler@nerdland.net>
 */

#ifndef FOOD_AMOUNT_CONTEXT_MENU_H_
#define FOOD_AMOUNT_CONTEXT_MENU_H_

#include "food_context_menu.h"
#include <QModelIndex>
#include "data/food_amount.h"

class FoodAmountContextMenu : public FoodContextMenu
{
  public:

    FoodAmountContextMenu(const QModelIndex& index, const FoodAmount& foodAmount,
                            QWidget* parent = 0);

    virtual ~FoodAmountContextMenu();

  private slots:

    virtual void actionTriggered(QAction* action);

  private:

     QModelIndex index;
     FoodAmount foodAmount;

     QAction* actViewNutritionInfo;
};

#endif /* FOOD_AMOUNT_CONTEXT_MENU_H_ */
