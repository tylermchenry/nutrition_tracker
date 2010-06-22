/*
 * nutrient_amount.cpp
 *
 *  Created on: May 14, 2010
 *      Author: tmchenry
 */

#include "nutrient_amount.h"
#include "nutrient.h"
#include <stdexcept>

NutrientAmount::NutrientAmount(const QSharedPointer<const Nutrient>& nutrient, double amount)
  : Amount<const Nutrient, NutrientAmount>(nutrient, amount,
      (nutrient == NULL ? QSharedPointer<const Unit>() : nutrient->getStandardUnit()))
{
}

NutrientAmount::~NutrientAmount()
{
}

double NutrientAmount::getAmountAsPercentRDI() const
{
  if ((getUnit() == NULL) || (getAmount() == 0) || (getNutrient()->getRDI() == 0)) {

    return 0;

  } else {

    // If the RDI is x units of Foo, and the current amount is y units of Bar,
    // then compute (y Bar * (Foo/Bar)) / (x Foo), where Foo/Bar is the conversion factor

    return (getAmount() * getUnit()->getConversionFactor(getNutrient()->getStandardUnit())) /
        getNutrient()->getRDI();
  }
}

void NutrientAmount::setAmountFromPercentRDI
  (double pct_rdi, const QSharedPointer<const Unit>& unit)
{
  const QSharedPointer<const Unit> targetUnit = (unit != NULL ? unit : getUnit());

  // If the RDI is x units of Foo and we want y fraction of the RDI in units of Bar, then
  // compute: y * (x Foo) * (Bar/Foo), where Bar/Foo is the conversion factor.

  setAmount(pct_rdi * getNutrient()->getRDI() *
            getNutrient()->getStandardUnit()->getConversionFactor(targetUnit), unit);
}

QString NutrientAmount::getSubstanceName(bool plural) const
{
  return QString("nutrient") + (plural ? "s" : "");
}
