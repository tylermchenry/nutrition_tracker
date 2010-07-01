/*
 * specialized_unit.cpp
 * Part of nutrition_tracker
 *
 *  Created on: Jun 25, 2010
 *      Author: Tyler McHenry <tyler@nerdland.net>
 */

#include "specialized_unit_impl.h"
#include "libnutrition/data/single_food.h"
#include <QVariant>
#include <QDebug>

SpecializedUnitImpl::SpecializedUnitImpl
  (int foodId, int sequence, const QString& name, double amountInGrams)
  : foodId(foodId), sequence(sequence), name(name), amountInGrams(amountInGrams)
{
}

SpecializedUnitImpl::~SpecializedUnitImpl()
{
}

QSharedPointer<Food> SpecializedUnitImpl::getFood() const
{
  return SingleFood::getSingleFood(foodId);
}
