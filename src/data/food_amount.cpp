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
  double scaleFactor = getAmount() / getFood()->getBaseAmount().getAmount(getUnit());

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
