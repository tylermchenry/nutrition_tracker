/*
 * specialized_unit.h
 * Part of nutrition_tracker
 *
 *  Created on: Jun 25, 2010
 *      Author: Tyler McHenry <tyler@nerdland.net>
 */

#ifndef SPECIALIZED_UNIT_IMPL_H_
#define SPECIALIZED_UNIT_IMPL_H_

#include "libnutrition/data/specialized_unit.h"
#include "libnutrition/data/food_amount.h"
#include "libnutrition/data/unit.h"
#include <QVector>
#include <QMap>
#include <QPair>
#include <QSharedPointer>

class SpecializedUnitImpl : virtual public SpecializedUnit
{
  public:

    SpecializedUnitImpl(int foodId, int sequence, const QString& name,
                           double amountInGrams);

    virtual ~SpecializedUnitImpl();

    virtual inline int getFoodId() const
      { return foodId; }

    virtual QSharedPointer<Food> getFood() const;

    virtual inline int getSequence() const
      { return sequence; }

    virtual inline QString getName() const
      { return name; }

    virtual inline QString getAbbreviation() const
      { return QString::number(amountInGrams, 'f', 0) + " g"; }

    virtual inline QString getNameAndAbbreviation() const
      { return getName() + " (" + getAbbreviation() + ")"; }

    virtual inline FoodAmount getBaseAmount() const
      { return FoodAmount(getFood(), amountInGrams, Unit::getUnit("g")); }

  private:

    int foodId;
    int sequence;
    QString name;
    double amountInGrams;
};

#endif /* SPECIALIZED_UNIT_IMPL_H_ */
