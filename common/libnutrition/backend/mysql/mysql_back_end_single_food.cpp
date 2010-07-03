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

QSharedPointer<SingleFood>  MySQLBackEnd::loadSingleFood(int id)
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
  // TODO: Replace
  food->saveToDatabase();
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
