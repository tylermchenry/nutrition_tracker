/*
 * composite_food.cpp
 *
 *  Created on: May 14, 2010
 *      Author: tmchenry
 */

#include "composite_food.h"
#include "data/nutrient_amount.h"

CompositeFood::CompositeFood(int id, const QString& name,
                             const QVector<FoodAmount>& components,
                             double weightAmount, double volumeAmount,
                             double quantityAmount, double servingAmount)
  : Food("COMPOSITE_" + QString::number(id), name, weightAmount, volumeAmount,
         quantityAmount, servingAmount), id(id), components(components)
{
}

CompositeFood::~CompositeFood()
{
}

QVector<FoodAmount> CompositeFood::getComponents() const
{
  return components;
}

QMap<QString, NutrientAmount> CompositeFood::getNutrients() const
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

QMap<QString, NutrientAmount>& CompositeFood::mergeNutrients
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
