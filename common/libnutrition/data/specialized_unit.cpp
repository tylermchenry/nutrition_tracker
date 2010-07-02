/*
 * specialized_unit.cpp
 * Part of nutrition_tracker
 *
 *  Created on: Jun 25, 2010
 *      Author: Tyler McHenry <tyler@nerdland.net>
 */

#include "specialized_unit.h"
#include "impl/specialized_unit_impl.h"
#include "data_cache.h"
#include "libnutrition/backend/back_end.h"
#include "single_food.h"

SpecializedUnit::SpecializedUnitIdTuple
  (SpecializedUnit::* const SpecializedUnit::cache_get_sort_key)() const =
     &SpecializedUnit::getSpecializedUnitIdTuple;

QSharedPointer<const SpecializedUnit>
  SpecializedUnit::getSpecializedUnit(int foodId, int sequence)
{
  SpecializedUnitIdTuple idTuple(foodId, sequence);

  if (DataCache<SpecializedUnit>::getInstance().contains(idTuple)) {
    return DataCache<SpecializedUnit>::getInstance().get(idTuple);
  } else {
    return BackEnd::getBackEnd()->loadSpecializedUnit(foodId, sequence);
  }
}

QVector<QSharedPointer<const SpecializedUnit> >
  SpecializedUnit::getAllSpecializedUnitsForFoodId(int foodId)
{
   QList<QSharedPointer<SpecializedUnit> > spUnits =
     BackEnd::getBackEnd()->loadAllSpecializedUnitsForFood(foodId);

   QVector<QSharedPointer<const SpecializedUnit> > spUnitsVec;

   for (QList<QSharedPointer<SpecializedUnit> >::const_iterator i = spUnits.begin();
        i != spUnits.end(); ++i)
   {
     spUnitsVec.push_back(*i);
   }

   return spUnitsVec;
}

SpecializedUnit::SpecializedUnitIdTuple::SpecializedUnitIdTuple
  (int fid, int s)
  : foodId(fid), sequence(s)
{
}

bool SpecializedUnit::SpecializedUnitIdTuple::operator<
  (const SpecializedUnitIdTuple& rhs) const
{
  return foodId < rhs.foodId ||
    (foodId == rhs.foodId && sequence < rhs.sequence);
}
