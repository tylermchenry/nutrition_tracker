/*
 * food.cpp
 *
 *  Created on: May 14, 2010
 *      Author: tmchenry
 */

#include "food.h"
#include <stdexcept>

Food::Food(const QString& id)
  : id(id), baseAmount(QSharedPointer<const Food>(this), 0, Unit::getPreferredUnit())
{
}

Food::~Food()
{
}

void Food::setBaseAmount(double amount, const QSharedPointer<const Unit>&  unit)
{
  if (unit != NULL) {
    baseAmount = FoodAmount(QSharedPointer<const Food>(this), amount, unit);
  } else {
    throw std::logic_error("Attempted to set the base amount of a food without a unit.");
  }
}

QVector<FoodAmount> Food::getComponents() const
{
  return QVector<FoodAmount>();
}


