/*
 * unit.cpp
 *
 *  Created on: May 14, 2010
 *      Author: tmchenry
 */

#include "unit.h"
#include "impl/unit_impl.h"
#include "data_cache.h"
#include "libnutrition/backend/back_end.h"
#include <stdexcept>

QString (Unit::* const Unit::cache_get_sort_key)() const = &Unit::getNameAndAbbreviation;

QSharedPointer<const Unit> Unit::getPreferredUnit(Dimensions::Dimension dimension)
{
  return getBasicUnit(dimension);
}

QSharedPointer<const Unit> Unit::getUnit(const QString& abbreviation)
{
  if (DataCache<Unit>::getInstance().contains(abbreviation)) {
    return DataCache<Unit>::getInstance().get(abbreviation);
  } else {
    return BackEnd::getBackEnd()->loadUnit(abbreviation);
  }
}

QVector<QSharedPointer<const Unit> > Unit::getAllUnits()
{
  static bool gotAll = false;

  if (gotAll) {
    // TODO: Make this method return a QList so this conversion is unnecessary
    static QVector<QSharedPointer<const Unit> > all;

    if (all.empty()) {
      all = DataCache<Unit>::getInstance().getAll().toVector();
    }

    return all;
  }

  QList<QSharedPointer<Unit> > units =
    BackEnd::getBackEnd()->loadAllUnits();

  gotAll = !units.empty();

  QVector<QSharedPointer<const Unit> > unitsVec;

  for (QList<QSharedPointer<Unit> >::const_iterator i = units.begin();
       i != units.end(); ++i)
  {
    unitsVec.push_back(*i);
  }

  return unitsVec;
}

QVector<QSharedPointer<const Unit> > Unit::getAllUnits(Dimensions::Dimension dimension)
{
  static QMap<Dimensions::Dimension, QVector<QSharedPointer<const Unit> > > all;

  if (!all[dimension].empty()) {
    return all[dimension];
  }

  QList<QSharedPointer<Unit> > units =
    BackEnd::getBackEnd()->loadAllUnits(dimension);

  for (QList<QSharedPointer<Unit> >::const_iterator i = units.begin();
  i != units.end(); ++i)
  {
    all[dimension].push_back(*i);
  }

  return all[dimension];
}

QSharedPointer<const Unit> Unit::getBasicUnit(Dimensions::Dimension dimension)
{
  static QMap<Dimensions::Dimension, QSharedPointer<const Unit> > basicUnits;

  if (basicUnits.contains(dimension)) {

    return basicUnits[dimension];

  } else {

    QSharedPointer<const Unit> basicUnit = BackEnd::getBackEnd()->loadBasicUnit(dimension);

    if (basicUnit) {
      basicUnits[dimension] = basicUnit;
    }

    return basicUnit;
  }
}

const QVector<Unit::Dimensions::Dimension>& Unit::Dimensions::getAllDimensions()
{
  static QVector<Dimension> allDimensions;

  // Order is significant here. Food amounts that can be expressed in multiple
  // dimensions will prefer the dimension that is highest in this list if no
  // other contextual information causes another dimension to be more preferred.

  if (allDimensions.empty()) {
    allDimensions.push_back(Dimensions::Weight);
    allDimensions.push_back(Dimensions::Volume);
    allDimensions.push_back(Dimensions::Quantity);
    allDimensions.push_back(Dimensions::Serving);
    allDimensions.push_back(Dimensions::Energy);
    allDimensions.push_back(Dimensions::IU);
  }

  return allDimensions;
}

Unit::Dimensions::Dimension Unit::Dimensions::fromHumanReadable(const QString& str)
{
  QString lowerStr = str.toLower();

  if (lowerStr == "weight")   return Dimensions::Weight;
  if (lowerStr == "volume")   return Dimensions::Volume;
  if (lowerStr == "quantity") return Dimensions::Quantity;
  if (lowerStr == "serving")  return Dimensions::Serving;
  if (lowerStr == "energy")   return Dimensions::Energy;
  if (lowerStr == "iu")   return Dimensions::IU;

  throw std::range_error("String does not describe a dimension.");
}

QString Unit::Dimensions::toHumanReadable(Dimension dim)
{
  switch (dim) {
    case Weight:   return "Weight";
    case Volume:   return "Volume";
    case Quantity: return "Quantity";
    case Serving:  return "Serving";
    case Energy:   return "Energy";
    case IU:       return "IU";
    default:      throw std::range_error("Dimension enumeration out of range.");
  }
}
