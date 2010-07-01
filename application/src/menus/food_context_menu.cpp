/*
 * food_context_menu.cpp
 *
 *  Created on: May 26, 2010
 *      Author: tyler
 */

#include "food_context_menu.h"
#include "libnutrition/data/food.h"
#include "libnutrition/data/food_collection.h"

FoodContextMenu::FoodContextMenu
  (const QModelIndex& index, QWidget* parent)
  : QMenu("Food", parent), index(index)
{
}

FoodContextMenu::~FoodContextMenu()
{
}
