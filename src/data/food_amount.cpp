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

QMap<QString, NutrientAmount> FoodAmount::getScaledNutrients() const
{
  // TODO: Cache this instead of recomputing every time

  if (getFood() == NULL) {
    throw std::logic_error("Attempted to scale the nutrients of an undefined food.");
  }

  QMap<QString, NutrientAmount> nutrients = getFood()->getNutrients();

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
    // A food need only define a base amount in a single dimension (although it
    // may define more). The UI should prevent the creation of FoodAmounts in
    // dimensions for which the underlying Food does not have a base amount.
    throw std::logic_error("Attempted to scale nutrients based on a dimension for which "
                            "the food does not have a base amount.");
  }

  double scaleFactor = getAmount() / baseAmount.getAmount(unit);

  for (QMap<QString, NutrientAmount>::iterator i = nutrients.begin();
      i != nutrients.end(); ++i)
  {
    (*i) *= scaleFactor;
  }

  return nutrients;
}

QString FoodAmount::getSubstanceName(bool plural) const
{
  return QString("food") + (plural ? "s" : "");
}
