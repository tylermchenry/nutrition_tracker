/*
 * single_food.cpp
 *
 *  Created on: May 14, 2010
 *      Author: tmchenry
 */

#include "single_food.h"

SingleFood::SingleFood(int id)
  : Food("SINGLE_" + QString::number(id)), id(id)
{
}

SingleFood::~SingleFood()
{
}

QMap<QString, NutrientAmount> SingleFood::getNutrients() const
{
  return nutrients;
}
