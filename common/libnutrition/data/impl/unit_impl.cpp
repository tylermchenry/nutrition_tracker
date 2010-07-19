/*
 * unit.cpp
 *
 *  Created on: May 14, 2010
 *      Author: tmchenry
 */

#include "unit_impl.h"
#include "libnutrition/proto/data/unit.pb.h"
#include <QVariant>
#include <QDebug>
#include <stdexcept>

UnitImpl::UnitImpl(const QString& abbreviation, const QString& name,
                      Dimensions::Dimension dimension, double basicConversionFactor)
  : abbreviation(abbreviation), name(name), dimension(dimension),
    basicConversionFactor(basicConversionFactor)
{
  qDebug() << "Created unit of " << Dimensions::toHumanReadable(dimension)
            << ", " << getNameAndAbbreviation() << ", factor = " << basicConversionFactor;
}

UnitImpl::~UnitImpl()
{
  qDebug() << "Unit abbreviated " << abbreviation << " was destroyed.";
}

double UnitImpl::getConversionFactor(const QSharedPointer<const Unit>& otherUnit) const
{
  const QSharedPointer<const Unit> basicUnit = getBasicUnit(dimension);

  if ((otherUnit == NULL) || (*otherUnit == *basicUnit)) {
    return basicConversionFactor;
  } else if (*otherUnit == *this) {
    return 1;
  } else if (*this == *basicUnit) {
    return 1 / otherUnit->getBasicConversionFactor();
  } else if (otherUnit->getDimension() != dimension) {
    throw std::logic_error("Attempted to convert units of different dimensions.");
  } else {
    // This unit is in terms of Foo, other unit is in terms of Bar, and basic unit is Baz.
    // The conversion factor z that we multiply x Foo by to get an equivalent y Bar is:
    // z = (Bar / Foo) = (Baz / Foo) * (Bar / Baz)
    return basicConversionFactor * basicUnit->getConversionFactor(otherUnit);
  }
}

UnitData UnitImpl::serialize() const
{
  UnitData udata;

  udata.set_abbreviation(abbreviation.toStdString());
  udata.set_name(name.toStdString());
  udata.set_dimension(static_cast<UnitData::Dimension>(dimension));
  udata.set_basicconversionfactor(basicConversionFactor);
  udata.set_isbasic((basicConversionFactor == 1));

  return udata;
}

