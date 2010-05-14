/*
 * nutrient_amount.cpp
 *
 *  Created on: May 14, 2010
 *      Author: tmchenry
 */

#include "nutrient_amount.h"
#include <stdexcept>

NutrientAmount::NutrientAmount(const Nutrient* nutrient, double amount)
  : Amount<Nutrient, NutrientAmount>
      (nutrient, amount, (nutrient == NULL ? NULL : nutrient->getStandardUnit()))
{
}

NutrientAmount::~NutrientAmount()
{
}

QString NutrientAmount::getSubstanceName(bool plural) const
{
  return QString("nutrient") + (plural ? "s" : "");
}
