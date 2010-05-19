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
    baseAmounts[Unit::Dimensions::Weight] = weightAmount;
  }

  if (volumeAmount > 0) {
    baseAmounts[Unit::Dimensions::Volume] = volumeAmount;
  }

  if (quantityAmount > 0) {
    baseAmounts[Unit::Dimensions::Quantity] = quantityAmount;
  }

  if (servingAmount > 0) {
    baseAmounts[Unit::Dimensions::Serving] = servingAmount;
  }
}

Food::~Food()
{
}

FoodAmount Food::getBaseAmount(Unit::Dimensions::Dimension dimension) const
{
  if (baseAmounts.contains(dimension)) {
    return FoodAmount(getCanonicalSharedPointer(), baseAmounts[dimension],
                       Unit::getPreferredUnit(dimension));
  } else {
    return FoodAmount();
  }
}

void Food::setBaseAmount(double amount, const QSharedPointer<const Unit>& unit)
{
  if (unit != NULL) {
    baseAmounts[unit->getDimension()] = amount * unit->getConversionFactor();
  } else {
    throw std::logic_error("Attempted to set the base amount of a food without a unit.");
  }
}

QVector<FoodAmount> Food::getComponents() const
{
  return QVector<FoodAmount>();
}


