/*
 * single_food.cpp
 *
 *  Created on: May 14, 2010
 *      Author: tmchenry
 */

#include "single_food.h"
#include <QVariant>
#include <QDebug>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlRecord>
#include <QtSql/QSqlField>
#include <QtSql/QSqlError>
#include <stdexcept>

QMap<int, QWeakPointer<SingleFood> > SingleFood::singleFoodCache;

int SingleFood::tempId = -1;

QSharedPointer<SingleFood> SingleFood::createNewFood()
{
  QSharedPointer<SingleFood> food(new SingleFood());
  singleFoodCache[food->getSingleFoodId()] = food;
  return food;
}

QSharedPointer<SingleFood> SingleFood::getSingleFood(int id)
{
  QSqlDatabase db = QSqlDatabase::database("nutrition_db");
  QSqlQuery query(db);

  if (singleFoodCache[id]) {
    return singleFoodCache[id].toStrongRef();
  }

  query.prepare("SELECT food_description.Food_Id, food_description.Entry_Src, "
                "       food_description.Long_Desc, food_description.Weight_g, "
                "       food_description.Volume_floz, food_description.Quantity, "
                "       food_description.Servings, nutrient_data.Nutr_Val, "
                "       nutrient_definition.Nutr_No, nutrient_definition.Category, "
                "       nutrient_definition.ShortName, nutrient_definition.RDI, "
                "       units.Unit, units.Type, units.Name, units.Factor,"
                "       group_description.FdGrp_Cd, group_description.FdGrp_Desc  "
                "FROM "
                "       food_description JOIN nutrient_data "
                "           ON food_description.Food_Id = nutrient_data.Food_Id "
                "   JOIN nutrient_definition "
                "           ON nutrient_data.Nutr_No = nutrient_definition.Nutr_No "
                "   JOIN units "
                "           ON nutrient_definition.Units = units.Unit "
                "   JOIN group_description "
                "           ON food_description.FdGrp_Cd = group_description.FdGrp_Cd "
                "WHERE "
                "       nutrient_definition.Category != 'Hidden' "
                "   AND food_description.Food_Id=:id "
                "ORDER BY "
                "   nutrient_definition.Category ASC, "
                "   nutrient_definition.Disp_Order ASC, "
                "   nutrient_definition.ShortName ASC");
  query.bindValue(":id", id);

  if (query.exec()) {
    qDebug() << "Executed query: " << query.executedQuery();
    return createSingleFoodFromQueryResults(query);
  } else {
    return QSharedPointer<SingleFood>();
  }
}

QSharedPointer<SingleFood> SingleFood::createSingleFoodFromQueryResults(QSqlQuery& query)
{
  QMap<QString, NutrientAmount> nutrients;

  while (query.next()) {
    QSharedPointer<const Nutrient> nutrient = Nutrient::createNutrientFromRecord(query.record());

    if (nutrient != NULL) {
      nutrients[nutrient->getId()] = NutrientAmount
        (nutrient, query.record().field("Nutr_Val").value().toDouble());
    }
  }

  if (query.first()) {
    const QSqlRecord& record = query.record();
    int id = record.field("Food_Id").value().toInt();
    if (!singleFoodCache[id]) {
      QSharedPointer<SingleFood> food
        (new SingleFood(id,
                        record.field("Long_Desc").value().toString(),
                        EntrySources::fromHumanReadable(record.field("Entry_Src").value().toString()),
                        Group::createGroupFromRecord(record),
                        nutrients,
                        record.field("Weight_g").value().toDouble(),
                        record.field("Volume_floz").value().toDouble(),
                        record.field("Quantity").value().toDouble(),
                        record.field("Servings").value().toDouble()));
      singleFoodCache[id] = food;
      return food;
    } else {
      return singleFoodCache[id].toStrongRef();
    }

  } else {
    return QSharedPointer<SingleFood>();
  }
}

SingleFood::SingleFood(int id, const QString& name, EntrySources::EntrySource entrySource,
                        const QSharedPointer<const Group>& group,
                        const QMap<QString, NutrientAmount>& nutrients,
                        double weightAmount, double volumeAmount,
                        double quantityAmount, double servingAmount)
  : Food("SINGLE_" + QString::number(id), name, weightAmount, volumeAmount,
         quantityAmount, servingAmount),
    id(id), entrySource(entrySource), group(group), nutrients(nutrients)
{
  qDebug() << "Created food ID " << id << " named " << name << " from "
           << EntrySources::toHumanReadable(entrySource)
           << " in group: " << group->getName() << " amounts: "
           << weightAmount << " g, " << volumeAmount << " fl oz "
           << quantityAmount << "qty, " << servingAmount << " srv.";

  qDebug() << "Nutrient Amounts:";

  for (QMap<QString, NutrientAmount>::const_iterator i = nutrients.begin();
       i != nutrients.end(); ++i)
  {
    qDebug() << i.value().getAmount() << " " << i.value().getUnit()->getNameAndAbbreviation()
             << " (" << i.value().getAmountAsPercentRDI() * 100 << "% RDI) of "
             << i.value().getNutrient()->getName();
  }
}

SingleFood::SingleFood()
  : Food("SINGLE_" + QString::number(tempId), "", 0, 0, 0, 0),
    id(tempId--), entrySource(EntrySources::Custom), group(Group::getDefaultGroup())
{
  qDebug() << "Created new food with temporary ID " << id;
}

SingleFood::~SingleFood()
{
}

QMap<QString, NutrientAmount> SingleFood::getNutrients() const
{
  return nutrients;
}

void SingleFood::setEntrySource(EntrySources::EntrySource source)
{
  this->entrySource = source;
}

void SingleFood::setGroup(const QSharedPointer<const Group>& group)
{
  if (group != NULL) {
    this->group = group;
  }
}

void SingleFood::setNutrient(const NutrientAmount& nutrientAmount)
{
  if (nutrientAmount.isDefined()) {

    // It's important to check that we're actually changing the value of the nutrient amount,
    // rather than overwriting it with itself. The reason is that nutrient amounts for foods
    // from the USDA source contain a lot of extra information about sample sizes, standard
    // deviations, etc, that won't be valid anymore if the value is changed. We need to
    // make sure that we only update the rows in the DB for which the value has actually
    // changed, in order to handle this properly.

    QString id = nutrientAmount.getNutrient()->getId();
    if (!nutrients[id].isDefined() ||
        (nutrients[id].getAmount() != nutrientAmount.getAmount(nutrients[id].getUnit())))
    {
      modifiedNutrients.insert(id);
      nutrients[id] = nutrientAmount;
    }
  }
}

void SingleFood::saveToDatabase()
{
  if (group == NULL) return;

  QSqlDatabase db = QSqlDatabase::database("nutrition_db");
  QSqlQuery query(db);

  // This needs to work either for a new food or an update to an existing food

  query.prepare("INSERT INTO food_description "
                "  (Food_Id, Entry_Src, FdGrp_Cd, Long_Desc, Weight_g, Volume_floz, Quantity, Servings) "
                "VALUES "
                "  (:id, :entrySource, :group, :name, :weight, :volume, :quantity, :servings) "
                "ON DUPLICATE KEY UPDATE"
                "  Entry_Src=:entrySource2, FdGrp_Cd=:group2, Long_Desc=:name2,"
                "  Weight_g=:weight2, Volume_floz=:volume2, Quantity=:quantity2, Servings=:servings2");

  query.bindValue(":id", (id >= 0 ? QVariant(id) : QVariant(QVariant::Int)));

  query.bindValue(":entrySource", EntrySources::toHumanReadable(entrySource));
  query.bindValue(":entrySource2", EntrySources::toHumanReadable(entrySource));
  query.bindValue(":group", group->getId());
  query.bindValue(":group2", group->getId());
  query.bindValue(":name", getName());
  query.bindValue(":name2", getName());

  bindBaseAmount(query, ":weight", Unit::Dimensions::Weight);
  bindBaseAmount(query, ":volume", Unit::Dimensions::Volume);
  bindBaseAmount(query, ":quantity", Unit::Dimensions::Quantity);
  bindBaseAmount(query, ":servings", Unit::Dimensions::Serving);

  bindBaseAmount(query, ":weight2", Unit::Dimensions::Weight);
  bindBaseAmount(query, ":volume2", Unit::Dimensions::Volume);
  bindBaseAmount(query, ":quantity2", Unit::Dimensions::Quantity);
  bindBaseAmount(query, ":servings2", Unit::Dimensions::Serving);

  if (!query.exec()) {
    qDebug() << "Query error: " << query.lastError();
    throw std::runtime_error("Failed to save food to database.");
  }

  if (id < 0) {
    int newId = query.lastInsertId().toInt();
    singleFoodCache[newId] = singleFoodCache[id];
    singleFoodCache.remove(id);
    qDebug() << "Assigned real ID " << newId << " to food with temp ID " << id;
    id = newId;
  }

  // Only change information for nutrients that have actually been modified
  // "4" below is the Source ID for "inputed" data. TODO: get rid of magic number

  if (entrySource == EntrySources::USDA) {
    // For USDA items, we want to do a REPLACE query in order to explicitly lose all of the
    // extra statistical data that would not apply to a modified value.

    query.prepare("REPLACE INTO nutrient_data "
                  "  (Food_Id, Nutr_No, Nutr_Val, Src_Cd) "
                  "VALUES "
                  "  (:id, :nutrient_id, :value, 4)");

  } else {
    // Otherwise, we want to do an INSERT-UPDATE query

    query.prepare("INSERT INTO nutrient_data "
                  "  (Food_Id, Nutr_No, Nutr_Val, Src_Cd) "
                  "VALUES "
                  "  (:id, :nutrient_id, :value, 4) "
                  "ON DUPLICATE KEY UPDATE"
                  "  Nutr_No=:nutrient_id2, Nutr_Val=:value2, Src_Cd=4");
  }

  for (QSet<QString>::const_iterator i = modifiedNutrients.begin(); i != modifiedNutrients.end(); ++i)
  {
    const NutrientAmount& amount = nutrients[*i];

    query.bindValue(":id", id);

    query.bindValue(":nutrient_id", amount.getNutrient()->getId());
    query.bindValue(":value", amount.getAmount(amount.getNutrient()->getStandardUnit()));

    if (entrySource != EntrySources::USDA) {
      query.bindValue(":nutrient_id2", amount.getNutrient()->getId());
      query.bindValue(":value2", amount.getAmount(amount.getNutrient()->getStandardUnit()));
    }

    if (!query.exec()) {
       throw std::runtime_error("Failed to save nutrient amount for food to database.");
    }
  }

  modifiedNutrients.clear();
}

QSharedPointer<Food> SingleFood::getCanonicalSharedPointer()
{
  return singleFoodCache[id].toStrongRef();
}

QSharedPointer<const Food> SingleFood::getCanonicalSharedPointer() const
{
  return singleFoodCache[id].toStrongRef();
}

SingleFood::EntrySources::EntrySource SingleFood::EntrySources::fromHumanReadable
  (const QString& str)
{
  QString lowerStr = str.toLower();

  if (lowerStr == "usda")   return EntrySources::USDA;
  if (lowerStr == "import") return EntrySources::Import;
  if (lowerStr == "custom") return EntrySources::Custom;

  throw std::range_error("String does not describe an entry source.");
}

QString SingleFood::EntrySources::toHumanReadable(EntrySource src)
{
  switch (src) {
    case USDA:   return "USDA";
    case Import: return "Import";
    case Custom: return "Custom";
    default:     throw std::range_error("EntrySource enumeration out of range.");
  }
}
