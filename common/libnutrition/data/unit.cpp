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

QVector<QSharedPointer<const Unit> > Unit::all;
QMap<Unit::Dimensions::Dimension, QVector<QSharedPointer<const Unit> > > Unit::allByDimension;
QMap<Unit::Dimensions::Dimension, QSharedPointer<const Unit> > Unit::basicUnits;

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
  if (all.isEmpty()) {

    BackEnd::getBackEnd()->loadAllUnits();

    // TODO: Make this method return a QList so this conversion is unnecessary
    all = DataCache<Unit>::getInstance().getAll().toVector();

    allByDimension.clear();
    basicUnits.clear();

    // Pre-fill the allByDimension and basicUnits caches
    for (QVector<QSharedPointer<const Unit> >::const_iterator i = all.begin();
         i != all.end(); ++i)
    {
      allByDimension[(*i)->getDimension()].append(*i);
      if ((*i)->getConversionFactor() == 1) {
        basicUnits[(*i)->getDimension()] = *i;
      }
    }
  }

  return all;
}

QVector<QSharedPointer<const Unit> > Unit::getAllUnits(Dimensions::Dimension dimension)
{
  if (allByDimension[dimension].empty()) {

    QList<QSharedPointer<Unit> > mutable_units =
      BackEnd::getBackEnd()->loadAllUnits(dimension);

    for (QList<QSharedPointer<Unit> >::const_iterator i = mutable_units.begin();
         i != mutable_units.end(); ++i)
    {
      allByDimension[dimension].push_back(*i);
    }
  }

  return allByDimension[dimension];
}

QSharedPointer<const Unit> Unit::getBasicUnit(Dimensions::Dimension dimension)
{
  if (!basicUnits.contains(dimension)) {

    QSharedPointer<const Unit> basicUnit = BackEnd::getBackEnd()->loadBasicUnit(dimension);

    if (basicUnit) {
      basicUnits[dimension] = basicUnit;
    }
  }

  return basicUnits[dimension];
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
