/*
 * unit.cpp
 *
 *  Created on: May 14, 2010
 *      Author: tmchenry
 */

#include "unit.h"
#include <QVariant>
#include <QDebug>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlRecord>
#include <QtSql/QSqlError>
#include <stdexcept>

QSharedPointer<const Unit> Unit::getPreferredUnit(Dimensions::Dimension dimension)
{
  return getBasicUnit(dimension);
}

QSharedPointer<const Unit> Unit::getUnit(const QString& abbreviation)
{
  QSqlDatabase db = QSqlDatabase::database("nutrition_db");
  QSqlQuery query(db);

  query.prepare("SELECT Unit, Type, Name, Factor FROM units WHERE Unit=:abbrev "
                "ORDER BY Name LIMIT 1");
  query.bindValue(":abbrev", abbreviation);

  QVector<QSharedPointer<const Unit> > units;

  if (query.exec()) {
    return createUnitFromQueryResults(query);
  } else {
    return QSharedPointer<const Unit>();
  }
}

QVector<QSharedPointer<const Unit> > Unit::getAllUnits()
{
  QSqlDatabase db = QSqlDatabase::database("nutrition_db");
  QSqlQuery query(db);

  if (query.exec("SELECT Unit, Type, Name, Factor FROM units ORDER BY Name")) {
    return createUnitsFromQueryResults(query);
  } else {
    return QVector<QSharedPointer<const Unit> >();
  }
}

QVector<QSharedPointer<const Unit> > Unit::getAllUnits(Dimensions::Dimension dimension)
{
  QSqlDatabase db = QSqlDatabase::database("nutrition_db");
  QSqlQuery query(db);

  query.prepare("SELECT Unit, Type, Name, Factor FROM units WHERE Type=:dim ORDER BY Name");
  query.bindValue(":dim", Dimensions::toHumanReadable(dimension));

  if (query.exec()) {
    return createUnitsFromQueryResults(query);
  } else {
    return QVector<QSharedPointer<const Unit> >();
  }
}

QSharedPointer<const Unit> Unit::createUnitFromQueryResults
  (const QSqlQuery& query, const QString& tablePrefix)
{
  QString prefix = (tablePrefix == "" ? "" : tablePrefix + ".");

  int abbrevField = query.record().indexOf(prefix + "Unit");
  int dimField = query.record().indexOf(prefix + "Type");
  int nameField = query.record().indexOf(prefix + "Name");
  int factorField = query.record().indexOf(prefix + "Factor");

  if (query.isValid()) {
    return QSharedPointer<const Unit>
      (new Unit(query.value(abbrevField).toString(),
                query.value(nameField).toString(),
                Dimensions::fromHumanReadable(query.value(dimField).toString()),
                query.value(factorField).toDouble()));
  } else {
    return QSharedPointer<const Unit>();
  }
}

QVector<QSharedPointer<const Unit> > Unit::createUnitsFromQueryResults
  (QSqlQuery& query, const QString& tablePrefix)
{
  QString prefix = (tablePrefix == "" ? "" : tablePrefix + ".");
  QVector<QSharedPointer<const Unit> > units;

  int abbrevField = query.record().indexOf(prefix + "Unit");
  int dimField = query.record().indexOf(prefix + "Type");
  int nameField = query.record().indexOf(prefix + "Name");
  int factorField = query.record().indexOf(prefix + "Factor");

  while (query.next()) {
    units.push_back(QSharedPointer<const Unit>
                    (new Unit(query.value(abbrevField).toString(),
                              query.value(nameField).toString(),
                              Dimensions::fromHumanReadable(query.value(dimField).toString()),
                              query.value(factorField).toDouble())));
  }

  return units;
}

Unit::Unit(const QString& abbreviation, const QString& name,
           Dimensions::Dimension dimension, double basicConversionFactor)
  : abbreviation(abbreviation), name(name), dimension(dimension),
    basicConversionFactor(basicConversionFactor)
{
  qDebug() << "Created unit of " << Dimensions::toHumanReadable(dimension)
           << ", " << getNameAndAbbreviation() << ", factor = " << basicConversionFactor;
}

Unit::~Unit()
{
}

double Unit::getConversionFactor(const QSharedPointer<const Unit>& otherUnit) const
{
  if ((otherUnit == NULL) || (otherUnit == getBasicUnit(dimension))) {
    return 1;
  } else if (otherUnit->getDimension() != dimension) {
    throw std::logic_error("Attempted to convert units of different dimensions.");
  } else {
    return basicConversionFactor /
      otherUnit->getConversionFactor(getBasicUnit(dimension));
  }
}

QSharedPointer<const Unit> Unit::getBasicUnit(Dimensions::Dimension dimension)
{
  static QMap<Dimensions::Dimension, QSharedPointer<const Unit> > basicUnits;

  if (basicUnits.contains(dimension)) {

    return basicUnits[dimension];

  } else {

    QSqlDatabase db = QSqlDatabase::database("nutrition_db");
    QSqlQuery query(db);

    query.prepare("SELECT Unit, Type, Name, Factor FROM units WHERE Type=:dim AND Factor=1 "
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

Unit::Dimensions::Dimension Unit::Dimensions::fromHumanReadable(const QString& str)
{
  QString lowerStr = str.toLower();

  if (lowerStr == "weight")   return Dimensions::Weight;
  if (lowerStr == "volume")   return Dimensions::Volume;
  if (lowerStr == "quantity") return Dimensions::Quantity;
  if (lowerStr == "serving")  return Dimensions::Serving;

  throw std::range_error("String does not describe a dimension.");
}

QString Unit::Dimensions::toHumanReadable(Dimension dim)
{
  switch (dim) {
    case Weight:   return "Weight";
    case Volume:   return "Volume";
    case Quantity: return "Quantity";
    case Serving:  return "Serving";
    default:      throw std::range_error("Dimension enumeration out of range.");
  }
}
