#include "mysql_back_end.h"
#include "libnutrition/data/data_cache.h"
#include "libnutrition/data/impl/specialized_unit_impl.h"
#include <QVariant>
#include <QDebug>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlRecord>
#include <QtSql/QSqlField>
#include <QtSql/QSqlError>
#include <stdexcept>

QSharedPointer<const SpecializedUnit>
  MySQLBackEnd::loadSpecializedUnit(int foodId, int sequence)
{
  QSqlQuery query(db);

  query.prepare("SELECT Food_Id, Seq, Amount, Msre_Desc, Gm_Wgt "
                " FROM weight WHERE Food_Id=:foodId AND Seq=:sequence "
                " ORDER BY Seq LIMIT 1");
  query.bindValue(":foodId", foodId);
  query.bindValue(":sequence", sequence);

  if (query.exec() && query.first()) {
    return createSpecializedUnitFromRecord(query.record());
  } else {
    return QSharedPointer<SpecializedUnit>();
  }
}

QList<QSharedPointer<SpecializedUnit> >
  MySQLBackEnd::loadAllSpecializedUnitsForFood(int foodId)
{
  QSqlQuery query(db);

  query.prepare("SELECT Food_Id, Seq, Amount, Msre_Desc, Gm_Wgt "
                 " FROM weight WHERE Food_Id=:foodId ORDER BY Seq");
  query.bindValue(":foodId", foodId);

  if (query.exec()) {
    return createSpecializedUnitsFromQueryResults(query);
  } else {
    return QList<QSharedPointer<SpecializedUnit> >();
  }
}

QList<QSharedPointer<SpecializedUnit> >
  MySQLBackEnd::createSpecializedUnitsFromQueryResults
    (QSqlQuery& query)
{
  QList<QSharedPointer<SpecializedUnit> > spUnits;

  while (query.next()) {
    spUnits.push_back(createSpecializedUnitFromRecord(query.record()));
  }

  return spUnits;
}

QSharedPointer<SpecializedUnit> MySQLBackEnd::createSpecializedUnitFromRecord
  (const QSqlRecord& record)
{
  if (!record.isEmpty()) {
    int foodId = record.field("Food_Id").value().toInt();
    int sequence = record.field("Seq").value().toInt();
    SpecializedUnit::SpecializedUnitIdTuple idTuple(foodId, sequence);
    QSharedPointer<SpecializedUnit> spUnit
      (new SpecializedUnitImpl(foodId, sequence,
                               record.field("Msre_Desc").value().toString(),
                               record.field("Gm_Wgt").value().toDouble()));
    DataCache<SpecializedUnit>::getInstance().insert(idTuple, spUnit);
    return spUnit;
  } else {
    return QSharedPointer<SpecializedUnit>();
  }
}

