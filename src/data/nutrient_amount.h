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

    NutrientAmount(const QSharedPointer<const Nutrient>& nutrient = QSharedPointer<const Nutrient>(),
                   double amount = 0);

    virtual ~NutrientAmount();

    inline QSharedPointer<const Nutrient> getNutrient() const { return getSubstance(); }

    double getAmountAsPercentRDI() const;

    void setAmountFromPercentRDI
      (double pct_rdi, const QSharedPointer<const Unit>& unit = QSharedPointer<const Unit>());

  protected:

    virtual QString getSubstanceName(bool plural) const;
};

#endif /* NUTRITIENT_AMOUNT_H_ */
