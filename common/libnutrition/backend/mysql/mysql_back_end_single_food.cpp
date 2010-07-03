#include "mysql_back_end.h"
#include "libnutrition/data/data_cache.h"
#include "libnutrition/data/impl/single_food_impl.h"
#include <QVariant>
#include <QDebug>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlRecord>
#include <QtSql/QSqlField>
#include <QtSql/QSqlError>
#include <stdexcept>

QSharedPointer<SingleFood> MySQLBackEnd::loadSingleFood(int id)
{
  QSqlQuery query(db);

  query.prepare("SELECT food_description.Food_Id, food_description.User_Id, "
                "       food_description.Entry_Src, "
                "       food_description.Long_Desc, food_description.Refuse,"
                "       food_description.Ref_desc, food_description.Weight_g, "
                "       food_description.Volume_floz, food_description.Quantity, "
                "       food_description.Servings, food_description.Fat_Factor,"
                "       food_description.CHO_Factor, food_description.Pro_Factor, "
                "       food_description.N_Factor, food_description.FdGrp_Cd, "
                "       nutrient_data.Nutr_No, nutrient_data.Nutr_Val "
                "FROM "
                "       food_description JOIN nutrient_data "
                "           ON food_description.Food_Id = nutrient_data.Food_Id "
                "WHERE "
                "   food_description.Food_Id=:id ");
  query.bindValue(":id", id);

  if (query.exec()) {
    qDebug() << "Executed query: " << query.executedQuery();
    return createSingleFoodFromQueryResults(query);
  } else {
    return QSharedPointer<SingleFood>();
  }
}


QMultiMap<QString, int>  MySQLBackEnd::loadSingleFoodNamesForUser(int userId)
{
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

void MySQLBackEnd::storeSingleFood(const QSharedPointer<SingleFood>& food)
{
  if (!food) {
    throw std::logic_error("Attempted to store a NULL food");
  } else if (!food->getGroup()) {
    throw std::logic_error("Attempted to store a food with an empty group.");
  }

  QSharedPointer<SingleFoodImpl> food_impl = food.dynamicCast<SingleFoodImpl>();

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

  query.bindValue
    (":id", (food->getSingleFoodId() >= 0 ? QVariant(food->getSingleFoodId()) :
                                            QVariant(QVariant::Int)));

  query.bindValue(":userId", food->getOwnerId());
  query.bindValue(":userId2", food->getOwnerId());
  query.bindValue(":entrySource", SingleFood::EntrySources::toHumanReadable
                      (food->getEntrySource()));
  query.bindValue(":entrySource2", SingleFood::EntrySources::toHumanReadable
                      (food->getEntrySource()));
  query.bindValue(":group", food->getGroup()->getId());
  query.bindValue(":group2", food->getGroup()->getId());
  query.bindValue(":name", food->getName());
  query.bindValue(":name2", food->getName());

  bindBaseAmount(food, query, ":weight", Unit::Dimensions::Weight);
  bindBaseAmount(food, query, ":volume", Unit::Dimensions::Volume);
  bindBaseAmount(food, query, ":quantity", Unit::Dimensions::Quantity);
  bindBaseAmount(food, query, ":servings", Unit::Dimensions::Serving);

  bindBaseAmount(food, query, ":weight2", Unit::Dimensions::Weight);
  bindBaseAmount(food, query, ":volume2", Unit::Dimensions::Volume);
  bindBaseAmount(food, query, ":quantity2", Unit::Dimensions::Quantity);
  bindBaseAmount(food, query, ":servings2", Unit::Dimensions::Serving);

  if (!query.exec()) {
    qDebug() << "Query error: " << query.lastError();
    throw std::runtime_error("Failed to save food to database.");
  }

  if (food->getSingleFoodId() < 0) {
    int newId = query.lastInsertId().toInt();
    DataCache<SingleFood>::getInstance().changeKey(food->getSingleFoodId(), newId);
    qDebug() << "Assigned real ID " << newId << " to food with temp ID "
              << food->getSingleFoodId();
    food_impl->setSingleFoodId(newId);
  }

  // Only change information for nutrients that have actually been modified
  // "4" below is the Source ID for "inputed" data. TODO: get rid of magic number

  if (food->getEntrySource() == SingleFood::EntrySources::USDA) {
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

  for (QSet<QString>::const_iterator i = food_impl->getModifiedNutrients().begin();
       i != food_impl->getModifiedNutrients().end(); ++i)
  {
    const NutrientAmount& amount = food_impl->getRawNutrients()[*i];

    query.bindValue(":id", food->getSingleFoodId());

    query.bindValue(":nutrient_id", amount.getNutrient()->getId());
    query.bindValue(":value", amount.getAmount(amount.getNutrient()->getStandardUnit()));

    if (food->getEntrySource() != SingleFood::EntrySources::USDA) {
      query.bindValue(":nutrient_id2", amount.getNutrient()->getId());
      query.bindValue(":value2", amount.getAmount(amount.getNutrient()->getStandardUnit()));
    }

    if (!query.exec()) {
      throw std::runtime_error("Failed to save nutrient amount for food to database.");
    }
  }

  food_impl->clearModifiedNutrients();
}

void MySQLBackEnd::deleteSingleFood(const QSharedPointer<SingleFood>& food)
{
  if (food) {
    deleteSingleFood(food->getSingleFoodId());
  } else {
    throw std::logic_error("Attempted to delete a NULL food");
  }
}

void MySQLBackEnd::deleteSingleFood(int id)
{
  // Silently ignore requests to delete temporaries; it's simpler to do that
  // than to force the client to check whether a food is temporary or not
  // every time it wants to delete something.

  if (id < 0) return;

  QSqlQuery query(db);

  query.prepare("DELETE FROM food_description WHERE Food_Id=:id");
  query.bindValue(":id", id);

  if (!query.exec()) {
    qDebug() << "Query error: " << query.lastError();
    throw std::runtime_error("Failed to delete food from database.");
  }

  DataCache<SingleFood>::getInstance().remove(id);
}

QSharedPointer<SingleFood> MySQLBackEnd::createSingleFoodFromQueryResults
  (QSqlQuery& query)
{
  QMap<QString, NutrientAmount> nutrients;

   while (query.next()) {
     QSharedPointer<const Nutrient> nutrient =
       Nutrient::getNutrient(query.record().field("Nutr_No").value().toString());

     if (nutrient != NULL) {
       nutrients[nutrient->getId()] = NutrientAmount
         (nutrient, query.record().field("Nutr_Val").value().toDouble());
     }
   }

   if (query.first()) {
     const QSqlRecord& record = query.record();
     int id = record.field("Food_Id").value().toInt();
     QSharedPointer<SingleFood> food
       (new SingleFoodImpl(id,
                           record.field("Long_Desc").value().toString(),
                           record.field("User_Id").value().toInt(),
                           SingleFood::EntrySources::fromHumanReadable
                           (record.field("Entry_Src").value().toString()),
                           Group::getGroup(record.field("FdGrp_Cd").value().toString()),
                           record.field("Refuse").value().toDouble(),
                           record.field("Ref_desc").value().toString(),
                           nutrients,
                           record.field("Weight_g").value().toDouble(),
                           record.field("Volume_floz").value().toDouble(),
                           record.field("Quantity").value().toDouble(),
                           record.field("Servings").value().toDouble(),
                           record.field("Fat_Factor").value().toDouble(),
                           record.field("CHO_Factor").value().toDouble(),
                           record.field("Pro_Factor").value().toDouble(),
                           record.field("N_Factor").value().toDouble()));
     DataCache<SingleFood>::getInstance().insert(id, food);
     return food;
   } else {
     return QSharedPointer<SingleFood>();
   }
}
