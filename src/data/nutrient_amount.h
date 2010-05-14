/*
 * nutrient_amount.h
 *
 *  Created on: May 14, 2010
 *      Author: tmchenry
 */

#ifndef NUTRIENT_AMOUNT_H_
#define NUTRIENT_AMOUNT_H_

#include "data/nutrient.h"
#include "data/amount.h"

class NutrientAmount : public Amount<Nutrient, NutrientAmount>
{
  public:

    NutrientAmount(const Nutrient* nutrient = NULL, double amount = 0);

    virtual ~NutrientAmount();

    inline const Nutrient* getNutrient() { return getSubstance(); }

  protected:

    virtual QString getSubstanceName(bool plural) const;
};

#endif /* NUTRITIENT_AMOUNT_H_ */
