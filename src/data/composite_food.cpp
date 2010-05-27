/*
 * composite_food.cpp
 *
 *  Created on: May 14, 2010
 *      Author: tmchenry
 */

#include "composite_food.h"
#include "data/nutrient_amount.h"
#include "data/food_amount.h"
#include "data/single_food.h"
#include <QVariant>
#include <QDebug>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlRecord>
#include <QtSql/QSqlField>
#include <QtSql/QSqlError>

QMap<int, QWeakPointer<CompositeFood> > CompositeFood::compositeFoodCache;

QSharedPointer<CompositeFood> CompositeFood::getCompositeFood(int id)
{
  QSqlDatabase db = QSqlDatabase::database("nutrition_db");
  QSqlQuery query(db);

  if (compositeFoodCache[id]) {
    return compositeFoodCache[id].toStrongRef();
  }

  query.prepare("SELECT composite_food.Composite_Id, composite_food.Description, "
                "       composite_food.Weight_g, composite_food.Volume_floz, "
                "       composite_food.Quantity, composite_food.Servings, "
                "       composite_food_link.CompositeLink_Id, composite_food_link.Contained_Type, "
                "       composite_food_link.Contained_Id, composite_food_link.Magnitude, "
                "       composite_food_link.IntrafoodOrder, "
                "       units.Unit, units.Type, "
                "       units.Name, units.Factor "
                "FROM"
                "        composite_food "
                "   JOIN composite_food_link "
                "        ON composite_food.Composite_Id = composite_food_link.Composite_Id "
                "   JOIN units "
                "        ON composite_food_link.Unit = units.Unit "
                "WHERE composite_food.Composite_Id = :id "
                "ORDER BY composite_food_link.IntrafoodOrder ASC");

  query.bindValue(":id", id);

  if (query.exec()) {
    qDebug() << "Executed query: " << query.executedQuery();
    return createCompositeFoodFromQueryResults(query);
  } else {
    qDebug() << "Query failed: " << query.lastError();
    return QSharedPointer<CompositeFood>();
  }
}

QSharedPointer<CompositeFood> CompositeFood::createCompositeFoodFromQueryResults
  (QSqlQuery& query)
{
  QSharedPointer<CompositeFood> food;

  if (query.first()) {

    const QSqlRecord& record = query.record();
    int id = record.field("Composite_Id").value().toInt();

    if (!compositeFoodCache[id]) {

      food = QSharedPointer<CompositeFood>
        (new CompositeFood(id,
                           record.field("Description").value().toString(),
                           record.field("Weight_g").value().toDouble(),
                           record.field("Volume_floz").value().toDouble(),
                           record.field("Quantity").value().toDouble(),
                           record.field("Servings").value().toDouble()));

      compositeFoodCache[id] = food;

    } else {
      return compositeFoodCache[id].toStrongRef();
    }
  }

  if (food && query.isActive()) {
    query.seek(-1); // Reset to before first record
    food->setComponents
      (createComponentsFromQueryResults
         (query, food, "CompositeLink_Id", "Intrafood_Order"));
  }

  return food;
}

CompositeFood::CompositeFood(int id, const QString& name,
                             const QSet<FoodComponent>& components,
                             double weightAmount, double volumeAmount,
                             double quantityAmount, double servingAmount)
  : FoodCollection("COMPOSITE_" + QString::number(id), name, components,
                   weightAmount, volumeAmount, quantityAmount, servingAmount),
    id(id)
{
}

CompositeFood::CompositeFood(int id, const QString& name,
                             double weightAmount, double volumeAmount,
                             double quantityAmount, double servingAmount)
  : FoodCollection("COMPOSITE_" + QString::number(id), name,
                   weightAmount, volumeAmount, quantityAmount, servingAmount),
    id(id)
{
}

CompositeFood::~CompositeFood()
{
}

void CompositeFood::saveToDatabase()
{
  // TODO: Implement
}

QSharedPointer<Food> CompositeFood::getCanonicalSharedPointer()
{
  return compositeFoodCache[id].toStrongRef();
}

QSharedPointer<const Food> CompositeFood::getCanonicalSharedPointer() const
{
  return compositeFoodCache[id].toStrongRef();
}

