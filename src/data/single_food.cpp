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

QSharedPointer<SingleFood> SingleFood::createNewFood
  (const QSharedPointer<const SingleFood>& copy)
{
  QSharedPointer<SingleFood> food(new SingleFood(copy));
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
                "       food_description.Servings, food_description.Fat_Factor,"
                "       food_description.CHO_Factor, food_description.Pro_Factor, "
                "       food_description.N_Factor, nutrient_data.Nutr_Val, "
                "       nutrient_definition.Nutr_No, nutrient_definition.Category, "
                "       nutrient_definition.ShortName, nutrient_definition.RDI, "
                "       units.Unit, units.Type, units.Name AS UnitName, units.Factor,"
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
                        record.field("Servings").value().toDouble(),
                        record.field("Fat_Factor").value().toDouble(),
                        record.field("CHO_Factor").value().toDouble(),
                        record.field("Pro_Factor").value().toDouble(),
                        record.field("N_Factor").value().toDouble()));
      singleFoodCache[id] = food;
      return food;
    } else {
      return singleFoodCache[id].toStrongRef();
    }

  } else {
    return QSharedPointer<SingleFood>();
  }
}

QMultiMap<QString, int> SingleFood::getFoodsForUser(int userId)
{
  QSqlDatabase db = QSqlDatabase::database("nutrition_db");
  QSqlQuery query(db);
  QMultiMap<QString, int> foods;

  query.prepare("SELECT food_description.Food_Id, food_description.Long_Desc "
                 "FROM   food_description "
                 "WHERE  food_description.User_Id = :userId");

  query.bindValue(":userId", userId);

  if (query.exec()) {
    while (query.next()) {
      const QSqlRecord& record = query.record();
      foods.insert(record.field("Long_Desc").value().toString(),
                   record.field("Food_Id").value().toInt());
    }
  } else {
    qDebug() << "Query failed: " << query.lastError();
  }

  return foods;
}

SingleFood::SingleFood(int id, const QString& name, EntrySources::EntrySource entrySource,
                        const QSharedPointer<const Group>& group,
                        const QMap<QString, NutrientAmount>& nutrients,
                        double weightAmount, double volumeAmount,
                        double quantityAmount, double servingAmount,
                        double calorieDensityFat, double calorieDensityCarbohydrate,
                        double calorieDensityProtien, double calorieDensityAlcohol)
  : Food("SINGLE_" + QString::number(id), name, weightAmount, volumeAmount,
         quantityAmount, servingAmount),
    id(id), entrySource(entrySource), group(group), nutrients(nutrients)
{
  qDebug() << "Created food ID " << id << " named " << name << " from "
           << EntrySources::toHumanReadable(entrySource)
           << " in group: " << group->getName() << " amounts: "
           << weightAmount << " g, " << volumeAmount << " fl oz "
           << quantityAmount << "qty, " << servingAmount << " srv."
           << " densities: fat =" << calorieDensityFat
           << " carb = " << calorieDensityCarbohydrate
           << " protein = " << calorieDensityProtien
           << " alcohol = " << calorieDensityAlcohol;

#ifdef VERBOSE_DEBUG
  qDebug() << "Nutrient Amounts:";

  for (QMap<QString, NutrientAmount>::const_iterator i = nutrients.begin();
       i != nutrients.end(); ++i)
  {
    qDebug() << i.value().getAmount() << " " << i.value().getUnit()->getNameAndAbbreviation()
             << " (" << i.value().getAmountAsPercentRDI() * 100 << "% RDI) of "
             << i.value().getNutrient()->getName();
  }
#endif

  setCalorieDensity(Nutrient::FAT_NAME, calorieDensityFat);
  setCalorieDensity(Nutrient::CARBOHYDRATE_NAME, calorieDensityCarbohydrate);
  setCalorieDensity(Nutrient::PROTEIN_NAME, calorieDensityProtien);
  setCalorieDensity(Nutrient::ALCOHOL_NAME, calorieDensityAlcohol);
  sanityCheckCalorieDensities();
}

SingleFood::SingleFood(const QSharedPointer<const SingleFood>& copy)
  : Food("SINGLE_" + QString::number(tempId), copy),
    id(tempId--), entrySource(copy ? copy->entrySource : EntrySources::Custom),
    group(copy ? copy->group : Group::getDefaultGroup())
{
  qDebug() << "Created new food with temporary ID " << id;

  if (copy) {
    calorieDensities = copy->calorieDensities;
  } else {
    setCalorieDensity(Nutrient::FAT_NAME, 0);
    setCalorieDensity(Nutrient::CARBOHYDRATE_NAME, 0);
    setCalorieDensity(Nutrient::PROTEIN_NAME, 0);
    setCalorieDensity(Nutrient::ALCOHOL_NAME, 0);
  }
}

SingleFood::~SingleFood()
{
}

QMap<QString, NutrientAmount> SingleFood::getNutrients() const
{
  return nutrients;
}

NutrientAmount SingleFood::getCaloriesFromNutrientId
  (const QString& nutrId) const
{
  QSharedPointer<const Nutrient> calories =
    Nutrient::getNutrientByName(Nutrient::CALORIES_NAME);

  if (nutrients.contains(nutrId) && calorieDensities.contains(nutrId)) {
    return NutrientAmount(calories,
                          nutrients[nutrId].getAmount(Unit::getUnit("g")) *
                          calorieDensities[nutrId]);
  } else if (nutrId == calories->getId() && nutrients.contains(nutrId)) {
    return NutrientAmount(calories, nutrients[nutrId].getAmount());
  } else {
    return NutrientAmount(calories, 0);
  }
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
      sanityCheckCalorieDensities();
    }
  }
}

void SingleFood::saveToDatabase()
{
  if (group == NULL) return;

  QSqlDatabase db = QSqlDatabase::database("nutrition_db");
  QSqlQuery query(db);

  // This needs to work either for a new food or an update to an existing food

  // Note that the calorie densities (factors) are not updated. Currently there
  // is no way to modify them, or even view them.

  // TODO: Save calorie density information if it becomes mutable

  query.prepare("INSERT INTO food_description "
                "  (Food_Id, User_Id, Entry_Src, FdGrp_Cd, Long_Desc, Weight_g, Volume_floz, Quantity, Servings) "
                "VALUES "
                "  (:id, :userId, :entrySource, :group, :name, :weight, :volume, :quantity, :servings) "
                "ON DUPLICATE KEY UPDATE"
                "  User_Id=:userId2, Entry_Src=:entrySource2, FdGrp_Cd=:group2, Long_Desc=:name2,"
                "  Weight_g=:weight2, Volume_floz=:volume2, Quantity=:quantity2, Servings=:servings2");

  query.bindValue(":id", (id >= 0 ? QVariant(id) : QVariant(QVariant::Int)));

  query.bindValue(":userId", 1); // TODO: Real User ID
  query.bindValue(":userId2", 1); // TODO: Real User ID
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

void SingleFood::setCalorieDensity(const QString& nutrientName, double density)
{
  QSharedPointer<const Nutrient> nutrient = Nutrient::getNutrientByName(nutrientName);
  if (nutrient) {
    calorieDensities[nutrient->getId()] = (density > 0) ? density :
      nutrient->getDefaultCalorieDensity();
  }
}

void SingleFood::sanityCheckCalorieDensities()
{
  QString caloriesId = Nutrient::getNutrientByName(Nutrient::CALORIES_NAME)->getId();

  if (nutrients.contains(caloriesId)) {

    NutrientAmount calories = nutrients[caloriesId];

    if (calories.getAmount() <= 0) return;

    NutrientAmount computedCalories = NutrientAmount(Nutrient::getNutrient(caloriesId), 0);

    NutrientAmount caloriesFromFat = getCaloriesFromNutrientName(Nutrient::FAT_NAME);
    NutrientAmount caloriesFromCarbs = getCaloriesFromNutrientName(Nutrient::CARBOHYDRATE_NAME);
    NutrientAmount caloriesFromProtein = getCaloriesFromNutrientName(Nutrient::PROTEIN_NAME);
    NutrientAmount caloriesFromAlcohol = getCaloriesFromNutrientName(Nutrient::ALCOHOL_NAME);

    if (caloriesFromFat.isDefined()) computedCalories += caloriesFromFat;
    if (caloriesFromCarbs.isDefined()) computedCalories += caloriesFromCarbs;
    if (caloriesFromProtein.isDefined()) computedCalories += caloriesFromProtein;
    if (caloriesFromAlcohol.isDefined()) computedCalories += caloriesFromAlcohol;

    // It's OK if the computed calories are smaller than the nominal calories;
    // that just means that some calories came from sources other than fat,
    // carbs, protein, and alcohol. But it's not OK if the computed calories are
    // larger than the nominal calories, since that means we are overestimating
    // one or more of the energy densities, which will screw up calories-from-
    // nutrient calculations elsewhere.

    // Since there's no way to determine exactly which densities are wrong and
    // by how much, we simply perform a uniform scaling to reconcile the
    // computed calories with the nominal calories.

    // This will normally happen with user-entered foods which are using the
    // USDA default calorie densities, which appear to be upper bounds. Foods
    // sourced from the USDA database have food-specific calorie densities that
    // are more accurate.

    double scaleFactor = calories.getAmount() / computedCalories.getAmount();

    if (scaleFactor < 1) {
      for (QMap<QString, double>::iterator i = calorieDensities.begin();
          i != calorieDensities.end(); ++i)
      {
        i.value() *= scaleFactor;
      }
    }
  }
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
