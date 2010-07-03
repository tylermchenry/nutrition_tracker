/*
 * food.cpp
 *
 *  Created on: May 14, 2010
 *      Author: tmchenry
 */

#include "food_impl.h"
#include "libnutrition/data/food_component.h"
#include "libnutrition/data/food_collection.h"
#include <QVariant>
#include <QDebug>
#include <stdexcept>

FoodImpl::FoodImpl
  (const QString& id, const QString& name, int ownerId,
   double weightAmount, double volumeAmount, double quantityAmount,
   double servingAmount)
: id(id), name(name), ownerId(ownerId)
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

FoodImpl::FoodImpl(const QString& id, const QSharedPointer<const Food>& copy)
  : id(id), name(copy ? copy->getName() : ""),
    ownerId(copy ? copy->getOwnerId() : User::getLoggedInUser()->getId())
{
  if (copy) {
    baseAmounts = copy->getBaseAmounts();
  }
}

FoodImpl::~FoodImpl()
{
  qDebug() << "Food ID " << id << " was destroyed.";
}

QList<Unit::Dimensions::Dimension> FoodImpl::getValidDimensions() const
{
  return baseAmounts.keys();
}

FoodAmount FoodImpl::getBaseAmount() const
{
  for (QVector<Unit::Dimensions::Dimension>::const_iterator i = Unit::Dimensions::getAllDimensions().begin();
      i != Unit::Dimensions::getAllDimensions().end(); ++i)
  {
    if (baseAmounts.contains(*i)) {
      return getBaseAmount(*i);
    }
  }

  return FoodAmount();
}

FoodAmount FoodImpl::getBaseAmount(Unit::Dimensions::Dimension dimension) const
{
  if (baseAmounts.contains(dimension)) {
    return FoodAmount(getCanonicalSharedPointer(), baseAmounts[dimension],
                       Unit::getPreferredUnit(dimension));
  } else {
    return FoodAmount();
  }
}

void FoodImpl::setBaseAmount(double amount, const QSharedPointer<const Unit>& unit)
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

void FoodImpl::setName(const QString& name)
{
  this->name = name;
}

NutrientAmount FoodImpl::getCaloriesFromNutrient
  (const QSharedPointer<const Nutrient>& nutrient) const
{
  if (nutrient) {
    return getCaloriesFromNutrientId(nutrient->getId());
  } else {
    return NutrientAmount();
  }
}

NutrientAmount FoodImpl::getCaloriesFromNutrientName
  (const QString& nutrName) const
{
  return getCaloriesFromNutrient(Nutrient::getNutrientByName(nutrName));
}

QList<FoodComponent> FoodImpl::getComponents() const
{
  return QList<FoodComponent>();
}

QVector<FoodAmount> FoodImpl::getComponentAmounts() const
{
  return QVector<FoodAmount>();
}
