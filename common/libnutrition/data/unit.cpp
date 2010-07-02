/*
 * unit.cpp
 *
 *  Created on: May 14, 2010
 *      Author: tmchenry
 */

#include "unit.h"
#include "impl/unit_impl.h"
#include "data_cache.h"
#include <QVariant>
#include <QDebug>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlRecord>
#include <QtSql/QSqlField>
#include <QtSql/QSqlError>
#include <stdexcept>

QString (Unit::* const Unit::cache_get_sort_key)() const = &Unit::getNameAndAbbreviation;

QSharedPointer<const Unit> Unit::getPreferredUnit(Dimensions::Dimension dimension)
{
  return getBasicUnit(dimension);
}

QSharedPointer<const Unit> Unit::getUnit(const QString& abbreviation)
{
  QSqlDatabase db = QSqlDatabase::database("nutrition_db");
  QSqlQuery query(db);

  if (DataCache<Unit>::getInstance().contains(abbreviation)) {
    return DataCache<Unit>::getInstance().get(abbreviation);
  }

  query.prepare("SELECT Unit, Type, Name AS UnitName, Factor FROM units WHERE Unit=:abbrev "
                "ORDER BY Name LIMIT 1");
  query.bindValue(":abbrev", abbreviation);

  if (query.exec() && query.first()) {
    return createUnitFromRecord(query.record());
  } else {
    return QSharedPointer<const Unit>();
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

  QSqlDatabase db = QSqlDatabase::database("nutrition_db");
  QSqlQuery query(db);

  if (query.exec("SELECT Unit, Type, Name AS UnitName, Factor FROM units ORDER BY Name")) {
    gotAll = true;
    return createUnitsFromQueryResults(query);
  } else {
    return QVector<QSharedPointer<const Unit> >();
  }
}

QVector<QSharedPointer<const Unit> > Unit::getAllUnits(Dimensions::Dimension dimension)
{
  QSqlDatabase db = QSqlDatabase::database("nutrition_db");
  QSqlQuery query(db);

  query.prepare("SELECT Unit, Type, Name AS UnitName, Factor FROM units WHERE Type=:dim ORDER BY Name");
  query.bindValue(":dim", Dimensions::toHumanReadable(dimension));

  if (query.exec()) {
    return createUnitsFromQueryResults(query);
  } else {
    return QVector<QSharedPointer<const Unit> >();
  }
}

QSharedPointer<const Unit> Unit::createUnitFromRecord(const QSqlRecord& record)
{
  if (!record.isEmpty()) {
    QString abbrev = record.field("Unit").value().toString();
    if (!DataCache<Unit>::getInstance().contains(abbrev)) {
      QSharedPointer<const Unit> unit
      (new UnitImpl(abbrev,
                    record.field("UnitName").value().toString(),
                    Dimensions::fromHumanReadable(record.field("Type").value().toString()),
                    record.field("Factor").value().toDouble()));
      DataCache<Unit>::getInstance().insert(abbrev, unit);
      return unit;
    } else {
      return DataCache<Unit>::getInstance().get(abbrev);
    }
  } else {
    return QSharedPointer<const Unit>();
  }
}

QVector<QSharedPointer<const Unit> > Unit::createUnitsFromQueryResults(QSqlQuery& query)
{
  QVector<QSharedPointer<const Unit> > units;

  while (query.next()) {
    units.push_back(createUnitFromRecord(query.record()));
  }

  return units;
}

QSharedPointer<const Unit> Unit::getBasicUnit(Dimensions::Dimension dimension)
{
  static QMap<Dimensions::Dimension, QSharedPointer<const Unit> > basicUnits;

  if (basicUnits.contains(dimension)) {

    return basicUnits[dimension];

  } else {

    QSqlDatabase db = QSqlDatabase::database("nutrition_db");
    QSqlQuery query(db);

    query.prepare("SELECT Unit, Type, Name AS UnitName, Factor FROM units WHERE Type=:dim AND Factor=1 "
                  "ORDER BY Name LIMIT 1");
    query.bindValue(":dim", Dimensions::toHumanReadable(dimension));

    QVector<QSharedPointer<const Unit> > units;

    if (query.exec()) {
      units = createUnitsFromQueryResults(query);
    }

    if (units.size() > 0) {
      basicUnits[dimension] = units[0];
      return units[0];
    } else {
      return QSharedPointer<const Unit>();
    }
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
