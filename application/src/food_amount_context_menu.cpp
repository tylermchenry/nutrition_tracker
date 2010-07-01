/*
 * food_amount_context_menu.cpp
 * Part of nutrition_tracker
 *
 *  Created on: May 31, 2010
 *      Author: Tyler McHenry <tyler@nerdland.net>
 */

#include "food_amount_context_menu.h"
#include "libnutrition/data/food.h"
#include "libnutrition/data/food_collection.h"

FoodAmountContextMenu::FoodAmountContextMenu
  (const QModelIndex& index, const FoodAmount& foodAmount, QWidget* parent)
  : FoodContextMenu(index, parent), foodAmount(foodAmount)
{
  actViewNutritionInfo = new QAction("View Nutrition Information...", this);
  addAction(actViewNutritionInfo);

  connect(this, SIGNAL(triggered(QAction*)), this, SLOT(actionTriggered(QAction*)));
}

FoodAmountContextMenu::~FoodAmountContextMenu()
{
}

void FoodAmountContextMenu::actionTriggered(QAction* action)
{
  if (action == actViewNutritionInfo) {
    emit viewNutritionInformation(getIndex(), foodAmount);
  }
}
