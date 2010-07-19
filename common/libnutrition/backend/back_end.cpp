/*
 * back_end.cpp
 *
 *  Created on: Jul 2, 2010
 *      Author: tyler
 */

#include "back_end.h"
#include <stdexcept>

QSharedPointer<BackEnd> BackEnd::backEnd;

QSharedPointer<Food> BackEnd::loadFood(BackEnd::FoodTypes::FoodType type, int id)
{
  switch (type)
  {
    case BackEnd::FoodTypes::SingleFood:
      return loadSingleFood(id);
      break;

    case BackEnd::FoodTypes::CompositeFood:
      return loadCompositeFood(id);
      break;

    case BackEnd::FoodTypes::Template:
      return loadTemplate(id);
      break;

    default:
      return QSharedPointer<Food>();
  }
}

QMultiMap<QString, QPair<BackEnd::FoodTypes::FoodType, int> >
  BackEnd::loadFoodNamesForUser
    (int userId, BackEnd::FoodTypes::FoodType type, bool includeExpired)
{
  QMultiMap<QString, QPair< BackEnd::FoodTypes::FoodType, int> > foods;
  QMultiMap<QString, int> rawFoods;

  if (type == BackEnd::FoodTypes::SingleFood) {
    rawFoods = loadSingleFoodNamesForUser(userId);
  } else if (type == BackEnd::FoodTypes::CompositeFood) {
    rawFoods = loadCompositeFoodNamesForUser(userId, includeExpired);
  } else if (type == BackEnd::FoodTypes::Template) {
    rawFoods = loadTemplateNamesForUser(userId);
  }

  for (QMultiMap<QString, int>::const_iterator i = rawFoods.begin();
  i != rawFoods.end(); ++i)
  {
    foods.insert(i.key(), qMakePair(type, i.value()));
  }

  return foods;
}

QString BackEnd::FoodTypes::toContainedType(FoodType type)
{
  switch (type)
  {
    case SingleFood:    return "Food";
    case CompositeFood: return "CompositeFood";
    case Template:      return "Template";
    default:           throw std::runtime_error("Cannot convert unknown food type");
  }
}

BackEnd::FoodTypes::FoodType BackEnd::FoodTypes::fromContainedType
  (const QString& food)
{
  QString foodLower = food.toLower();

  if       (foodLower == "food")           return SingleFood;
  else if (foodLower == "compositefood")  return CompositeFood;
  else if (foodLower == "template")       return Template;
  else     throw std::runtime_error("Cannot convert unknown contained type " +
                                    food.toStdString());
}
