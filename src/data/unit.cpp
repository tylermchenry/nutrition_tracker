/*
 * unit.cpp
 *
 *  Created on: May 14, 2010
 *      Author: tmchenry
 */

#include "unit.h"
#include <stdexcept>

const Unit* Unit::getPreferredUnit(Dimensions::Dimension dimension)
{
  return getBasicUnit(dimension);
}

Unit::Unit(const QString& abbrevation)
  : abbreviation(abbreviation), basicConversionFactor(1)
{
}

Unit::~Unit()
{
}

double Unit::getConversionFactor(const Unit* otherUnit) const
{
  if (otherUnit == NULL) {
    return 1;
  } else if (otherUnit->getDimension() != dimension) {
    throw std::logic_error("Attempted to convert units of different dimensions.");
  } else if (otherUnit == getBasicUnit(dimension)) {
    return basicConversionFactor;
  } else {
    return basicConversionFactor /
      otherUnit->getConversionFactor(getBasicUnit(dimension));
  }
}

const Unit* Unit::getBasicUnit(Dimensions::Dimension dimension)
{
  return NULL; // TODO: Implement
}
