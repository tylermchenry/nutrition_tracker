/*
 * nutrient.cpp
 *
 *  Created on: May 14, 2010
 *      Author: tmchenry
 */

#include "nutrient.h"
#include <QVariant>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlRecord>
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

  if (query.exec()) {
    return createNutrientFromQueryResults(query, "nutrient_definition");
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
    nutrients = createNutrientsFromQueryResults(query, "nutrient_definition");
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
    nutrients = createNutrientsFromQueryResults(query, "nutrient_definition");
  }

  return nutrients;
}

QSharedPointer<const Nutrient> Nutrient::createNutrientFromQueryResults
  (const QSqlQuery& query, const QString& tablePrefix)
{
  QString prefix = (tablePrefix == "" ? "" : tablePrefix + ".");

  int idField = query.record().indexOf(prefix + "Nutr_No");
  int categoryField = query.record().indexOf(prefix + "Category");
  int nameField = query.record().indexOf(prefix + "ShortName");
  int rdiField = query.record().indexOf(prefix + "RDI");

  if (query.isValid()) {
    return QSharedPointer<const Nutrient>
      (new Nutrient(query.value(idField).toString(),
                    query.value(nameField).toString(),
                    Categories::fromHumanReadable(query.value(categoryField).toString()),
                    Unit::createUnitFromQueryResults(query, "units"),
                    query.value(rdiField).toDouble()));
  } else {
    return QSharedPointer<const Nutrient>();
  }
}

QVector<QSharedPointer<const Nutrient> > Nutrient::createNutrientsFromQueryResults
  (QSqlQuery& query, const QString& tablePrefix)
{
  QString prefix = (tablePrefix == "" ? "" : tablePrefix + ".");
  QVector<QSharedPointer<const Nutrient> > nutrients;

  int idField = query.record().indexOf(prefix + "Nutr_No");
  int categoryField = query.record().indexOf(prefix + "Category");
  int nameField = query.record().indexOf(prefix + "ShortName");
  int rdiField = query.record().indexOf(prefix + "RDI");

  while (query.next()) {
    nutrients.push_back
      (QSharedPointer<const Nutrient>
        (new Nutrient(query.value(idField).toString(),
                      query.value(nameField).toString(),
                      Categories::fromHumanReadable(query.value(categoryField).toString()),
                      Unit::createUnitFromQueryResults(query, "units"),
                      query.value(rdiField).toDouble())));
  }

  return nutrients;
}

Nutrient::Nutrient(const QString& id, const QString& name, Categories::Category category,
                   const QSharedPointer<const Unit>& standardUnit, double rdi)
  : id(id), name(name), category(category), standardUnit(standardUnit), rdi(rdi)
{
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
