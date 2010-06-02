/*
 * nutrient_amount.h - Data object for an amount of a nutrient
 *
 * This file is part of Nutrition Tracker.
 *
 * Nutrition Tracker is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Nutrition Tracker is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Nutrition Tracker.  If not, see <http://www.gnu.org/licenses/>.
 *
 * Copyright © 2010 Tyler McHenry <tyler@nerdland.net>
 */

#ifndef NUTRIENT_AMOUNT_H_
#define NUTRIENT_AMOUNT_H_

#include "data/nutrient.h"
#include "data/amount.h"

/* NutrientAmount is an Amount where the substance is a Nutrient.
 *
 * In addition to the features of the Amount class, the NutrientAmount class
 * provides a facilities for retrieving and modifying the amount in terms of
 * the USDA reference daily intake (RDI) of the nutrient. Not all nutrients
 * have RDIs. If these methods are used on an amount of a nutrient without
 * an RDI, the amount that is retrieved or set will be 0.
 */
class NutrientAmount : public Amount<Nutrient, NutrientAmount>
{
  public:

    NutrientAmount(const QSharedPointer<const Nutrient>& nutrient =
                       QSharedPointer<const Nutrient>(),
                     double amount = 0);

    virtual ~NutrientAmount();

    inline QSharedPointer<const Nutrient> getNutrient() const
      { return getSubstance(); }

    double getAmountAsPercentRDI() const;

    void setAmountFromPercentRDI
      (double pct_rdi, const QSharedPointer<const Unit>& unit =
          QSharedPointer<const Unit>());

  protected:

    virtual QString getSubstanceName(bool plural) const;
};

#endif /* NUTRITIENT_AMOUNT_H_ */
