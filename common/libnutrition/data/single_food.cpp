/*
 * single_food.cpp
 *
 *  Created on: May 14, 2010
 *      Author: tmchenry
 */

#include "single_food.h"
#include "impl/single_food_impl.h"
#include "data_cache.h"
#include "libnutrition/backend/back_end.h"

int SingleFood::tempId = -1;

QSharedPointer<SingleFood> SingleFood::createNewFood
  (const QSharedPointer<const SingleFood>& copy)
{
  QSharedPointer<SingleFood> food(new SingleFoodImpl(copy));
  DataCache<SingleFood>::getInstance().insert(food->getSingleFoodId(), food);
  return food;
}

QSharedPointer<SingleFood> SingleFood::getSingleFood(int id)
{
  if (DataCache<SingleFood>::getInstance().contains(id)) {
    return DataCache<SingleFood>::getInstance().get(id);
  } else {
    return BackEnd::getBackEnd()->loadSingleFood(id);
  }
}

QMultiMap<QString, int> SingleFood::getFoodsForUser(int userId)
{
  return BackEnd::getBackEnd()->loadSingleFoodNamesForUser(userId);
}

QSharedPointer<Food> SingleFood::getCanonicalSharedPointer() const
{
  return DataCache<SingleFood>::getInstance().get(getSingleFoodId());
}

SingleFood::EntrySources::EntrySource SingleFood::EntrySources::fromHumanReadable
  (const QString& str)
{
  QString lowerStr = str.toLower();

  if (lowerStr == "usda")   return EntrySources::USDA;
  if (lowerStr == "import") return EntrySources::Import;
  if (lowerStr == "custom") return EntrySources::Custom;

  throw std::range_error("String does not describe an entry source.");
}

QString SingleFood::EntrySources::toHumanReadable(EntrySource src)
{
  switch (src) {
    case USDA:   return "USDA";
    case Import: return "Import";
    case Custom: return "Custom";
    default:     throw std::range_error("EntrySource enumeration out of range.");
  }
}
