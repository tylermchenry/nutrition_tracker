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

QMap<int, QWeakPointer<SingleFood> > SingleFood::singleFoodCache;

QSharedPointer<SingleFood> SingleFood::getSingleFood(int id)
{
  QSqlDatabase db = QSqlDatabase::database("nutrition_db");
  QSqlQuery query(db);

  query.prepare("SELECT food_description.Food_Id, food_description.Entry_Src, "
                "       food_description.Long_Desc, food_description.Weight_g, "
                "       food_description.Volume_floz, food_description.Quantity, "
                "       food_description.Servings, nutrient_data.Nutr_Val, "
                "       nutrient_definition.Nutr_No, nutrient_definition.Category, "
                "       nutrient_definition.ShortName, nutrient_definition.RDI, "
                "       units.Unit, units.Type, units.Name, units.Factor "
                "FROM "
                "       food_description JOIN nutrient_data "
                "           ON food_description.Food_Id = nutrient_data.Food_Id "
                "   JOIN nutrient_definition "
                "           ON nutrient_data.Nutr_No = nutrient_definition.Nutr_No "
                "   JOIN units "
                "           ON nutrient_definition.Units = units.Unit "
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
                        const QMap<QString, NutrientAmount>& nutrients,
                        double weightAmount, double volumeAmount,
                        double quantityAmount, double servingAmount)
  : Food("SINGLE_" + QString::number(id), name, weightAmount, volumeAmount,
         quantityAmount, servingAmount), id(id), entrySource(entrySource), nutrients(nutrients)
{
  qDebug() << "Created food ID " << id << " named " << name << " from "
           << EntrySources::toHumanReadable(entrySource) << " amounts: "
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

SingleFood::~SingleFood()
{
}

QMap<QString, NutrientAmount> SingleFood::getNutrients() const
{
  return nutrients;
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
