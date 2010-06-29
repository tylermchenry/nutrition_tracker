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

const QString Nutrient::CALORIES_NAME = "Calories";
const QString Nutrient::FAT_NAME = "Total Fat";
const QString Nutrient::CARBOHYDRATE_NAME = "Total Carbohydrate";
const QString Nutrient::PROTEIN_NAME = "Protein";
const QString Nutrient::ALCOHOL_NAME = "Alcohol";

QMap<QString, QSharedPointer<const Nutrient> > Nutrient::nutrientCache;
QMap<QString, QSharedPointer<const Nutrient> > Nutrient::nutrientCacheByName;

QSharedPointer<const Nutrient> Nutrient::getNutrientByName(const QString& name)
{
  if (!nutrientCacheByName.contains(name)) {
    getAllNutrients();
  }

  if (nutrientCacheByName.contains(name)) {
    return nutrientCacheByName[name];
  } else {
    return QSharedPointer<const Nutrient>();
  }
}

QSharedPointer<const Nutrient> Nutrient::getNutrient(const QString& id)
{
  QSqlDatabase db = QSqlDatabase::database("nutrition_db");
  QSqlQuery query(db);

  if (nutrientCache.contains(id)) {
    return nutrientCache[id];
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
    // TODO: Make this method return a QList so this conversion is unnecessary
    return nutrientCacheByName.values().toVector();
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
    if (!nutrientCache.contains(id)) {
      QSharedPointer<const Nutrient> nutrient
      (new Nutrient(id,
                    record.field("ShortName").value().toString(),
                    Categories::fromHumanReadable(record.field("Category").value().toString()),
                    Unit::createUnitFromRecord(record),
                    record.field("RDI").value().toDouble()));
      qDebug() << "Added nutrient named " << nutrient->getName() << " to cache at ID " << id;
      nutrientCache[id] = nutrient;
      nutrientCacheByName[nutrient->getName()] = nutrient;
      return nutrient;
    } else {
      return nutrientCache[id];
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
  qDebug() << "Nutrient ID " << id << " was destroyed.";
}

double Nutrient::getDefaultCalorieDensity() const
{
  // These are the reference energy densities given by the USDA. Normally, these
  // are given as 9, 4, and 7 kilocalories/gram, but the extra two decimal
  // places were obtained by converting from the more accurate measures of
  // 37, 17, and 29 kilojoules/gram.

  // Most USDA-entered single foods provide calorie density information specific
  // to that food, which is more accurate than these default values, and that
  // information will be used in preference to this when available. In general,
  // these numbers are OVER-estimates compared to the per-food densities.

  // TODO: There are a few other escoteric nutrients that contribute
  // to calories. Add them here.

  static const double FAT_DENSITY = 8.84;
  static const double CARBOHYDRATE_DENSITY = 4.06;
  static const double PROTEIN_DENSITY = 4.06;
  static const double ALCOHOL_DENSITY = 6.93;

  if (name == FAT_NAME) {
    return FAT_DENSITY;
  } else if (name == CARBOHYDRATE_NAME) {
    return CARBOHYDRATE_DENSITY;
  } else if (name == PROTEIN_NAME) {
    return PROTEIN_DENSITY;
  } else if (name == ALCOHOL_NAME) {
    return ALCOHOL_DENSITY;
  } else if (name == CALORIES_NAME) {
    return 1;
  } else {
    return 0;
  }
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
