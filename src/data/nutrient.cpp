/*
 * nutrient.cpp
 *
 *  Created on: May 14, 2010
 *      Author: tmchenry
 */

#include "nutrient.h"
#include <QVariant>
#include <QDebug>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlRecord>
#include <QtSql/QSqlField>
#include <QtSql/QSqlError>
#include <stdexcept>

QSharedPointer<const Nutrient> Nutrient::getNutrient(const QString& id)
{
  QSqlDatabase db = QSqlDatabase::database("nutrition_db");
  QSqlQuery query(db);

  query.prepare("SELECT nutrient_definition.Nutr_No, nutrient_definition.Category, "
                "  nutrient_definition.ShortName, nutrient_definition.RDI, "
                "  units.Unit, units.Type, units.Name, units.Factor "
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
  QSqlDatabase db = QSqlDatabase::database("nutrition_db");
  QSqlQuery query(db);

  query.prepare("SELECT nutrient_definition.Nutr_No, nutrient_definition.Category, "
                "  nutrient_definition.ShortName, nutrient_definition.RDI, "
                "  units.Unit, units.Type, units.Name, units.Factor "
                "FROM nutrient_definition JOIN units ON nutrient_definition.Units = units.Unit "
                "ORDER BY nutrient_definition.category ASC, nutrient_definition.Disp_Order ASC, "
                "  nutrient_definition.ShortName ASC");

  QVector<QSharedPointer<const Nutrient> > nutrients;

  if (query.exec()) {
    nutrients = createNutrientsFromQueryResults(query);
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
                "  units.Unit, units.Type, units.Name, units.Factor "
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
    return QSharedPointer<const Nutrient>
      (new Nutrient(record.field("Nutr_No").value().toString(),
                    record.field("ShortName").value().toString(),
                    Categories::fromHumanReadable(record.field("Category").value().toString()),
                    Unit::createUnitFromRecord(record),
                    record.field("RDI").value().toDouble()));
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

Nutrient::Nutrient(const QString& id, const QString& name, Categories::Category category,
                   const QSharedPointer<const Unit>& standardUnit, double rdi)
  : id(id), name(name), category(category), standardUnit(standardUnit), rdi(rdi)
{
  qDebug() << "Created nutrient ID " << id << " named " << name << " in category "
           << Categories::toHumanReadable(category) << ". Standard unit is "
           << standardUnit->getNameAndAbbreviation() << " RDI is " << rdi;
}

Nutrient::~Nutrient()
{
}

Nutrient::Categories::Category Nutrient::Categories::fromHumanReadable(const QString& str)
{
  QString lowerStr = str.toLower();

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
    case Basic:      return "Basic";
    case Vitamin:    return "Vitamin";
    case Mineral:    return "Mineral";
    case Additional: return "Additional";
    case Hidden:     return "Hidden";
    default:         throw std::range_error("Category enumeration out of range.");
  }
}
