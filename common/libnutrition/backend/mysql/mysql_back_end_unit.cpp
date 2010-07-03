#include "mysql_back_end.h"
#include "libnutrition/data/data_cache.h"
#include "libnutrition/data/impl/unit_impl.h"
#include <QVariant>
#include <QDebug>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlRecord>
#include <QtSql/QSqlField>
#include <QtSql/QSqlError>
#include <stdexcept>

QSharedPointer<const Unit> MySQLBackEnd::loadUnit(const QString& abbr)
{
  QSqlQuery query(db);

  query.prepare("SELECT Unit, Type, Name AS UnitName, Factor FROM units WHERE Unit=:abbrev "
                "ORDER BY Name LIMIT 1");
  query.bindValue(":abbrev", abbr);

  if (query.exec() && query.first()) {
    return createUnitFromRecord(query.record());
  } else {
    return QSharedPointer<Unit>();
  }
}

QList<QSharedPointer<Unit> > MySQLBackEnd::loadAllUnits()
{
  QSqlQuery query(db);

  if (query.exec("SELECT Unit, Type, Name AS UnitName, Factor FROM units ORDER BY Name")) {
    return createUnitsFromQueryResults(query);
  } else {
    return QList<QSharedPointer<Unit> >();
  }
}

QList<QSharedPointer<Unit> > MySQLBackEnd::loadAllUnits
  (Unit::Dimensions::Dimension dimension)
{
  QSqlQuery query(db);

  query.prepare("SELECT Unit, Type, Name AS UnitName, Factor FROM units WHERE Type=:dim ORDER BY Name");
  query.bindValue(":dim", Unit::Dimensions::toHumanReadable(dimension));

  if (query.exec()) {
    return createUnitsFromQueryResults(query);
  } else {
    return QList<QSharedPointer<Unit> >();
  }
}

QSharedPointer<const Unit> MySQLBackEnd::loadBasicUnit
  (Unit::Dimensions::Dimension dimension)
{
  QSqlQuery query(db);

  query.prepare("SELECT Unit, Type, Name AS UnitName, Factor FROM units WHERE Type=:dim AND Factor=1 "
                "ORDER BY Name LIMIT 1");
  query.bindValue(":dim", Unit::Dimensions::toHumanReadable(dimension));

  QList<QSharedPointer<Unit> > units;

  if (query.exec()) {
    units = createUnitsFromQueryResults(query);
  }

  if (units.size() > 0) {
    return units[0];
  } else {
    return QSharedPointer<const Unit>();
  }
}

QList<QSharedPointer<Unit> > MySQLBackEnd::createUnitsFromQueryResults
  (QSqlQuery& query)
{
  QList<QSharedPointer<Unit> > units;

  while (query.next()) {
    units.push_back(createUnitFromRecord(query.record()));
  }

  return units;
}

QSharedPointer<const Unit> MySQLBackEnd::createUnitFromRecordOrCache
  (const QSqlRecord& record)
{
  QString abbrev = record.field("Unit").value().toString();
  if (DataCache<Unit>::getInstance().contains(abbrev)) {
    return DataCache<Unit>::getInstance().get(abbrev);
  } else {
    return createUnitFromRecord(record);
  }
}

QSharedPointer<Unit> MySQLBackEnd::createUnitFromRecord
  (const QSqlRecord& record)
{
  if (!record.isEmpty()) {
    QString abbrev = record.field("Unit").value().toString();
    QSharedPointer<Unit> unit
      (new UnitImpl(abbrev,
                    record.field("UnitName").value().toString(),
                    Unit::Dimensions::fromHumanReadable
                      (record.field("Type").value().toString()),
                    record.field("Factor").value().toDouble()));
    DataCache<Unit>::getInstance().insert(abbrev, unit);
    return unit;
  } else {
    return QSharedPointer<Unit>();
  }
}
