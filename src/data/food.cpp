/*
 * food.cpp
 *
 *  Created on: May 14, 2010
 *      Author: tmchenry
 */

#include "food.h"
#include <QVariant>
#include <QDebug>
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

QList<Unit::Dimensions::Dimension> Food::getValidDimensions() const
{
  return baseAmounts.keys();
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
    if (amount > 0) {
      baseAmounts[unit->getDimension()] = amount * unit->getConversionFactor();
    } else {
      baseAmounts.remove(unit->getDimension());
    }
  } else {
    throw std::logic_error("Attempted to set the base amount of a food without a unit.");
  }
}

void Food::setName(const QString& name)
{
  this->name = name;
}

QVector<FoodAmount> Food::getComponents() const
{
  return QVector<FoodAmount>();
}

void Food::bindBaseAmount
  (QSqlQuery& query, const QString& placeholder, Unit::Dimensions::Dimension dimension) const
{
  FoodAmount amount = getBaseAmount(dimension);

  if (amount.isDefined()) {
    query.bindValue(placeholder, amount.getAmount(Unit::getPreferredUnit(dimension)));
  } else {
    query.bindValue(placeholder, QVariant(QVariant::Double)); // Binds "NULL"
  }
}
