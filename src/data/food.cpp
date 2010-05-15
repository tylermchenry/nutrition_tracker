/*
 * food.cpp
 *
 *  Created on: May 14, 2010
 *      Author: tmchenry
 */

#include "food.h"
#include <stdexcept>

Food::Food(const QString& id, const QString& name, double weightAmount,
           double volumeAmount, double quantityAmount, double servingAmount)
: id(id), name(name)
{
  if (weightAmount > 0) {
    baseAmounts[Unit::Dimensions::Weight] =
      FoodAmount(QSharedPointer<const Food>(this), weightAmount,
                 Unit::getPreferredUnit(Unit::Dimensions::Weight));
  }

  if (volumeAmount > 0) {
    baseAmounts[Unit::Dimensions::Volume] =
      FoodAmount(QSharedPointer<const Food>(this), volumeAmount,
                 Unit::getPreferredUnit(Unit::Dimensions::Volume));
  }

  if (quantityAmount > 0) {
    baseAmounts[Unit::Dimensions::Quantity] =
      FoodAmount(QSharedPointer<const Food>(this), quantityAmount,
                 Unit::getPreferredUnit(Unit::Dimensions::Quantity));
  }

  if (servingAmount > 0) {
    baseAmounts[Unit::Dimensions::Serving] =
      FoodAmount(QSharedPointer<const Food>(this), servingAmount,
                 Unit::getPreferredUnit(Unit::Dimensions::Serving));
  }
}

Food::~Food()
{
}

FoodAmount Food::getBaseAmount(Unit::Dimensions::Dimension dimension) const
{
  if (baseAmounts.contains(dimension)) {
    return baseAmounts[dimension];
  } else {
    return FoodAmount();
  }
}

void Food::setBaseAmount(double amount, const QSharedPointer<const Unit>& unit)
{
  if (unit != NULL) {
    baseAmounts[unit->getDimension()] =
      FoodAmount(QSharedPointer<const Food>(this), amount, unit);
  } else {
    throw std::logic_error("Attempted to set the base amount of a food without a unit.");
  }
}

QVector<FoodAmount> Food::getComponents() const
{
  return QVector<FoodAmount>();
}


