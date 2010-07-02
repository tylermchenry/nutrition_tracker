/*
 * nutrient.cpp
 *
 *  Created on: May 14, 2010
 *      Author: tmchenry
 */

#include "nutrient.h"
#include "impl/nutrient_impl.h"
#include "data_cache.h"
#include <QVariant>
#include <QDebug>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlRecord>
#include <QtSql/QSqlField>
#include <QtSql/QSqlError>
#include <stdexcept>

const QString Nutrient::CALORIES_NAME = "Calories";
const QString Nutrient::FAT_NAME = "Total Fat";
const QString Nutrient::CARBOHYDRATE_NAME = "Total Carbohydrate";
const QString Nutrient::PROTEIN_NAME = "Protein";
const QString Nutrient::ALCOHOL_NAME = "Alcohol";

QString (Nutrient::* const Nutrient::cache_get_sort_key)() const = &Nutrient::getName;

QSharedPointer<const Nutrient> Nutrient::getNutrientByName(const QString& name)
{
  static QMap<QString, QSharedPointer<const Nutrient> > nutrientsByName;

  if (nutrientsByName.empty()) {
    QVector<QSharedPointer<const Nutrient> > all = getAllNutrients();

    for (QVector<QSharedPointer<const Nutrient> >::const_iterator i = all.begin();
        i != all.end(); ++i)
    {
      if (*i) {
        nutrientsByName.insert((*i)->getName(), *i);
      }
    }
  }

  if (nutrientsByName.contains(name)) {
    return nutrientsByName[name];
  } else {
    return QSharedPointer<const Nutrient>();
  }
}

QSharedPointer<const Nutrient> Nutrient::getNutrient(const QString& id)
{
  QSqlDatabase db = QSqlDatabase::database("nutrition_db");
  QSqlQuery query(db);

  if (DataCache<Nutrient>::getInstance().contains(id)) {
    return DataCache<Nutrient>::getInstance().get(id);
  }

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
    return QSharedPointer<const Nutrient>();
  }
}

QVector<QSharedPointer<const Nutrient> > Nutrient::getAllNutrients()
{
  static bool gotAll = false;

  if (gotAll) {
    static QVector<QSharedPointer<const Nutrient> > all;

    if (all.empty()) {
      // TODO: Make this method return a QList so this conversion is unnecessary
      all = DataCache<Nutrient>::getInstance().getAll().toVector();
    }

    return all;
  }

  QSqlDatabase db = QSqlDatabase::database("nutrition_db");
  QSqlQuery query(db);

  query.prepare("SELECT nutrient_definition.Nutr_No, nutrient_definition.Category, "
                "  nutrient_definition.ShortName, nutrient_definition.RDI, "
                "  units.Unit, units.Type, units.Name AS UnitName, units.Factor "
                "FROM nutrient_definition JOIN units ON nutrient_definition.Units = units.Unit "
                "ORDER BY nutrient_definition.category ASC, nutrient_definition.Disp_Order ASC, "
                "  nutrient_definition.ShortName ASC");

  QVector<QSharedPointer<const Nutrient> > nutrients;

  if (query.exec()) {
    nutrients = createNutrientsFromQueryResults(query);
    gotAll = true;
  }

  return nutrients;
}

QVector<QSharedPointer<const Nutrient> > Nutrient::getAllNutrients
  (Categories::Category category)
{
  QSqlDatabase db = QSqlDatabase::database("nutrition_db");
  QSqlQuery query(db);

  query.prepare("SELECT nutrient_definition.Nutr_No, nutrient_definition.Category, "
                "  nutrient_definition.ShortName, nutrient_definition.RDI, "
                "  units.Unit, units.Type, units.Name AS UnitName, units.Factor "
                "FROM nutrient_definition JOIN units ON nutrient_definition.Units = units.Unit "
                "WHERE nutrient_definition.Category=:category "
                "ORDER BY nutrient_definition.category ASC, nutrient_definition.Disp_Order ASC, "
                "  nutrient_definition.ShortName ASC");

  query.bindValue(":category", Categories::toHumanReadable(category));

  QVector<QSharedPointer<const Nutrient> > nutrients;

  if (query.exec()) {
    nutrients = createNutrientsFromQueryResults(query);
  }

  return nutrients;
}

QSharedPointer<const Nutrient> Nutrient::createNutrientFromRecord
  (const QSqlRecord& record)
{
  if (!record.isEmpty()) {
    QString id = record.field("Nutr_No").value().toString();
    if (!DataCache<Nutrient>::getInstance().contains(id)) {
      QSharedPointer<const Nutrient> nutrient
      (new NutrientImpl(id,
                        record.field("ShortName").value().toString(),
                        Categories::fromHumanReadable(record.field("Category").value().toString()),
                        Unit::createUnitFromRecord(record),
                        record.field("RDI").value().toDouble()));
      qDebug() << "Added nutrient named " << nutrient->getName() << " to cache at ID " << id;
      DataCache<Nutrient>::getInstance().insert(id, nutrient);
      return nutrient;
    } else {
      return DataCache<Nutrient>::getInstance().get(id);
    }
  } else {
    return QSharedPointer<const Nutrient>();
  }
}

QVector<QSharedPointer<const Nutrient> > Nutrient::createNutrientsFromQueryResults
  (QSqlQuery& query)
{
  QVector<QSharedPointer<const Nutrient> > nutrients;

  while (query.next()) {
    nutrients.push_back(createNutrientFromRecord(query.record()));
  }

  return nutrients;
}

Nutrient::Categories::Category Nutrient::Categories::fromHumanReadable(const QString& str)
{
  QString lowerStr = str.toLower();

  if (lowerStr == "energy")     return Categories::Energy;
  if (lowerStr == "basic")      return Categories::Basic;
  if (lowerStr == "vitamin")    return Categories::Vitamin;
  if (lowerStr == "mineral")    return Categories::Mineral;
  if (lowerStr == "additional") return Categories::Additional;
  if (lowerStr == "hidden")     return Categories::Hidden;

  qDebug() << "Bad category: " << str;
  throw std::range_error("String does not describe a Category.");
}

QString Nutrient::Categories::toHumanReadable(Category cat)
{
  switch (cat) {
    case Energy:     return "Energy";
    case Basic:      return "Basic";
    case Vitamin:    return "Vitamin";
    case Mineral:    return "Mineral";
    case Additional: return "Additional";
    case Hidden:     return "Hidden";
    default:         throw std::range_error("Category enumeration out of range.");
  }
}
