/*
 * single_food.cpp
 *
 *  Created on: May 14, 2010
 *      Author: tmchenry
 */

#include "single_food.h"
#include "impl/single_food_impl.h"
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
  QSharedPointer<SingleFood> food(new SingleFoodImpl(copy));
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

QSharedPointer<SingleFood> SingleFood::createSingleFoodFromQueryResults(QSqlQuery& query)
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
    if (!singleFoodCache[id]) {
      QSharedPointer<SingleFood> food
        (new SingleFoodImpl(id,
                            record.field("Long_Desc").value().toString(),
                            record.field("User_Id").value().toInt(),
                            EntrySources::fromHumanReadable(record.field("Entry_Src").value().toString()),
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

QSharedPointer<Food> SingleFood::getCanonicalSharedPointer() const
{
  return singleFoodCache[getSingleFoodId()].toStrongRef();
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
