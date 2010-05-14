/*
 * compound_food.cpp
 *
 *  Created on: May 14, 2010
 *      Author: tmchenry
 */

#include "compound_food.h"

CompoundFood::CompoundFood(int id)
  : Food("COMPOUND_" + QString::number(id)), id(id)
{
  // TODO Auto-generated constructor stub

}

CompoundFood::~CompoundFood()
{
  // TODO Auto-generated destructor stub
}

QVector<FoodAmount> CompoundFood::getComponents() const
{
  return components;
}

QMap<QString, NutrientAmount> CompoundFood::getNutrients() const
{
  // TODO: Intelligently cache instead of recomputing each time

  QMap<QString, NutrientAmount> nutrients;

  for (QVector<FoodAmount>::const_iterator i = components.begin();
       i != components.end(); ++i)
  {
    mergeNutrients(nutrients, i->getScaledNutrients());
  }

  return nutrients;
}

QMap<QString, NutrientAmount>& CompoundFood::mergeNutrients
  (QMap<QString, NutrientAmount>& nutrients,
   const QMap<QString, NutrientAmount>& newNutrients)
{
  for (QMap<QString, NutrientAmount>::const_iterator i = newNutrients.begin();
       i != newNutrients.end(); ++i)
  {
    nutrients[i.key()] += i.value();
  }

  return nutrients;
}
