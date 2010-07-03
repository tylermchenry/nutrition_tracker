#include "mysql_back_end.h"
#include "libnutrition/data/data_cache.h"
#include "libnutrition/data/impl/nutrient_impl.h"
#include <QVariant>
#include <QDebug>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlRecord>
#include <QtSql/QSqlField>
#include <QtSql/QSqlError>
#include <stdexcept>

QSharedPointer<const Nutrient> MySQLBackEnd::loadNutrient(const QString& id)
{
  QSqlQuery query(db);

  query.prepare("SELECT nutrient_definition.Nutr_No, nutrient_definition.Category, "
                "  nutrient_definition.ShortName, nutrient_definition.RDI, "
                "  units.Unit, units.Type, units.Name AS UnitName, units.Factor "
                "FROM nutrient_definition JOIN units ON nutrient_definition.Units = units.Unit "
                "WHERE Nutr_No=:id "
                "ORDER BY nutrient_definition.category ASC, nutrient_definition.Disp_Order ASC, "
                "  nutrient_definition.ShortName ASC "
                "LIMIT 1");
  query.bindValue(":id", id);

  if (query.exec() && query.first()) {
    return createNutrientFromRecord(query.record());
  } else {
    return QSharedPointer<Nutrient>();
  }
}

QList<QSharedPointer<Nutrient> > MySQLBackEnd::loadAllNutrients()
{
  QSqlQuery query(db);

  query.prepare("SELECT nutrient_definition.Nutr_No, nutrient_definition.Category, "
                "  nutrient_definition.ShortName, nutrient_definition.RDI, "
                "  units.Unit, units.Type, units.Name AS UnitName, units.Factor "
                "FROM nutrient_definition JOIN units ON nutrient_definition.Units = units.Unit "
                "ORDER BY nutrient_definition.category ASC, nutrient_definition.Disp_Order ASC, "
                "  nutrient_definition.ShortName ASC");

  QList<QSharedPointer<Nutrient> > nutrients;

  if (query.exec()) {
    nutrients = createNutrientsFromQueryResults(query);
  }

  return nutrients;
}

QList<QSharedPointer<Nutrient> > MySQLBackEnd::loadAllNutrients
  (Nutrient::Categories::Category category)
{
  QSqlQuery query(db);

  query.prepare("SELECT nutrient_definition.Nutr_No, nutrient_definition.Category, "
                "  nutrient_definition.ShortName, nutrient_definition.RDI, "
                "  units.Unit, units.Type, units.Name AS UnitName, units.Factor "
                "FROM nutrient_definition JOIN units ON nutrient_definition.Units = units.Unit "
                "WHERE nutrient_definition.Category=:category "
                "ORDER BY nutrient_definition.category ASC, nutrient_definition.Disp_Order ASC, "
                "  nutrient_definition.ShortName ASC");

  query.bindValue(":category", Nutrient::Categories::toHumanReadable(category));

  QList<QSharedPointer<Nutrient> > nutrients;

  if (query.exec()) {
    nutrients = createNutrientsFromQueryResults(query);
  }

  return nutrients;
}

QList<QSharedPointer<Nutrient> > MySQLBackEnd::createNutrientsFromQueryResults
  (QSqlQuery& query)
{
  QList<QSharedPointer<Nutrient> > nutrients;

  while (query.next()) {
    nutrients.push_back(createNutrientFromRecord(query.record()));
  }

  return nutrients;
}

QSharedPointer<Nutrient> MySQLBackEnd::createNutrientFromRecord
  (const QSqlRecord& record)
{
  if (!record.isEmpty()) {
    QString id = record.field("Nutr_No").value().toString();
    QSharedPointer<Nutrient> nutrient
      (new NutrientImpl(id,
                        record.field("ShortName").value().toString(),
                        Nutrient::Categories::fromHumanReadable
                        (record.field("Category").value().toString()),
                        createUnitFromRecordOrCache(record),
                        record.field("RDI").value().toDouble()));
    qDebug() << "Added nutrient named " << nutrient->getName() << " to cache at ID " << id;
    DataCache<Nutrient>::getInstance().insert(id, nutrient);
    return nutrient;
  } else {
    return QSharedPointer<Nutrient>();
  }
}
