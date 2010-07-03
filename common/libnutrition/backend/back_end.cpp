/*
 * back_end.cpp
 *
 *  Created on: Jul 2, 2010
 *      Author: tyler
 */

#include "back_end.h"
#include <stdexcept>

QSharedPointer<BackEnd> BackEnd::backEnd;

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
