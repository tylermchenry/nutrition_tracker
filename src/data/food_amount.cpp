/*
 * food_amount.cpp
 *
 *  Created on: May 14, 2010
 *      Author: tmchenry
 */

#include "food_amount.h"
#include "food.h"
#include <stdexcept>

FoodAmount::FoodAmount(const QSharedPointer<const Food>& food, double amount,
                       const QSharedPointer<const Unit>& unit)
  : Amount<Food, FoodAmount>(food, amount, unit)
{
}

FoodAmount::~FoodAmount()
{
}

double FoodAmount::getAmount(const QSharedPointer<const Unit>& otherUnit) const
{
  if (!otherUnit || !isDefined() ||
      otherUnit->getDimension() == getUnit()->getDimension())
  {
    return Amount<Food, FoodAmount>::getAmount(otherUnit);
  }
  else
  {
    // Unlike generic Amounts, which cannot meaningfully convert from units in
    // one dimension to units in another, foods can be specified in multiple
    // dimensions, allowing for such a conversion. For example, if a food's
    // entry describes (equivalently) 2 C or 100 g, then it should be possible
    // to determine how many grams 3 Tbsp of the food weigh.

    // This is done by first finding the conversion factor from the unit that
    // the amount is already in to the base unit of the dimension that we
    // wish to convert to. Then, we do the normal intra-dimension conversion
    // from the base unit of the target dimension to the ultimately desired unit.

    FoodAmount baseAmountInSourceDimension =
      getFood()->getBaseAmount(getUnit()->getDimension());

    // baseAmountInSourceDimension must be defined, or something is already
    // seriously wrong, and there should have been an exception already.

    FoodAmount baseAmountInTargetDimension =
      getFood()->getBaseAmount(otherUnit->getDimension());

    if (baseAmountInTargetDimension.isDefined()) {

      double dimensionConversionFactor = baseAmountInTargetDimension.getAmount() /
          baseAmountInSourceDimension.getAmount();

      FoodAmount amountInTargetBaseUnit
      (getFood(), getAmount() * dimensionConversionFactor,
       baseAmountInTargetDimension.getUnit());

      return amountInTargetBaseUnit.getAmount(otherUnit);

    } else {

      throw std::logic_error("Attempted to get an amount of a food in an inappropriate dimension.");
    }
  }
}

QMap<QString, NutrientAmount> FoodAmount::getScaledNutrients() const
{
  // TODO: Cache this instead of recomputing every time

  if (getFood() == NULL) {
    throw std::logic_error("Attempted to scale the nutrients of an undefined food.");
  }

  QMap<QString, NutrientAmount> nutrients = getFood()->getNutrients();

  double scaleFactor = getScaleFactor();

  for (QMap<QString, NutrientAmount>::iterator i = nutrients.begin();
      i != nutrients.end(); ++i)
  {
    (*i) *= scaleFactor;
  }

  return nutrients;
}

QVector<FoodAmount> FoodAmount::getScaledComponents() const
{
  // See getScaledNutrients for logic description

  if (getFood() == NULL) {
    throw std::logic_error("Attempted to scale the components of an undefined food.");
  }

  QVector<FoodAmount> components = getFood()->getComponentAmounts();

  double scaleFactor = getScaleFactor();

  for (QVector<FoodAmount>::iterator i = components.begin(); i != components.end(); ++i)
  {
    (*i) *= scaleFactor;
  }

  return components;
}

NutrientAmount FoodAmount::getScaledCaloriesFromNutrient
  (const QSharedPointer<const Nutrient>& nutr) const
{
  if (nutr) {
    return getScaledCaloriesFromNutrientId(nutr->getId());
  } else {
    return NutrientAmount();
  }
}

NutrientAmount FoodAmount::getScaledCaloriesFromNutrientName
  (const QString& nutrName) const
{
  return getScaledCaloriesFromNutrient(Nutrient::getNutrientByName(nutrName));
}

NutrientAmount FoodAmount::getScaledCaloriesFromNutrientId
  (const QString& nutrId) const
{
  if (getFood() == NULL) {
    throw std::logic_error("Attempted to scale the nutrients of an undefined food.");
  }

  return getFood()->getCaloriesFromNutrientId(nutrId) * getScaleFactor();
}

QString FoodAmount::getSubstanceName(bool plural) const
{
  return QString("food") + (plural ? "s" : "");
}

double FoodAmount::getScaleFactor() const
{
  if (getFood() == NULL) {
    throw std::logic_error("Attempted to scale the an undefined food.");
  }

  // This FoodAmount object represents x amount of food in units of Foo.
  // In order to scale the nutrients, we need to know what fraction of the entry
  // for this food this x Foo amount represents.

  // To determine this, we need to get the base amount for the food entry in terms
  // of some unit Bar that is of the same dimension as Foo (e.g. Foo and Bar must
  // both be weights, or both volumes, etc.)

  // Once the base amount b Bar is determined, then the scale factor y can be computed
  // as: y = (x Foo) / (b Bar * (Foo/Bar))

  // So we approach this by first obtaining baseAmount (b Bar), and then converting
  // it to be in terms of Foo units by calling baseAmount.getAmount(unit). Then the
  // division is performed to get the scale factor.

  const QSharedPointer<const Unit> unit = getUnit();
  const QSharedPointer<const Food> food = getFood();
  FoodAmount baseAmount = food->getBaseAmount(unit->getDimension());

  if (!baseAmount.isDefined()) {
    throw std::logic_error("Attempted to scale based on a dimension for which "
        "the food does not have a base amount.");
  }

  return getAmount() / baseAmount.getAmount(unit);
}
