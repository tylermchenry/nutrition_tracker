/*
 * mysql_back_end.cpp
 *
 *  Created on: Jul 2, 2010
 *      Author: tyler
 */

#include "mysql_back_end.h"
#include <QDebug>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlRecord>
#include <QtSql/QSqlField>
#include <QtSql/QSqlError>
#include <stdexcept>

MySQLBackEnd::MySQLBackEnd(const QString& db_name)
  : db(QSqlDatabase::database(db_name, true))
{
  if (!db.open()) {
    throw std::runtime_error
      ("Could not open database " + db_name.toStdString() + " as back end.");
  }
}

MySQLBackEnd::~MySQLBackEnd()
{
  db.close();
}

QMultiMap<QString, QPair<BackEnd::FoodTypes::FoodType, int> >
  MySQLBackEnd::loadFoodNamesForUser(int userId, bool includeExpired)
{
  return loadFoodNamesForUser(userId, BackEnd::FoodTypes::SingleFood, includeExpired) +
    loadFoodNamesForUser(userId, BackEnd::FoodTypes::CompositeFood, includeExpired) +
    loadFoodNamesForUser(userId, BackEnd::FoodTypes::Template, includeExpired);
}

void MySQLBackEnd::fillReadOnlyCaches()
{
  loadAllUnits();
  loadAllGroups();
  loadAllNutrients();
}

QPair<QList<QSharedPointer<Unit> >,
      QList<QSharedPointer<SpecializedUnit> > >
  MySQLBackEnd::loadAllUnitsForFood(int foodId)
{
  return qMakePair(loadAllUnits(), loadAllSpecializedUnitsForFood(foodId));
}

QList<FoodComponent> MySQLBackEnd::createComponentsFromQueryResults
  (QSqlQuery& query, const QSharedPointer<FoodCollection>& containingCollection,
   const QString& componentIdField, const QString& componentOrderField) const
{
  QList<FoodComponent> components;
  int order = -1;

  while (query.next()) {

    const QSqlRecord& record = query.record();

    if (record.field(componentIdField).value().isNull()) continue;

    int containedId = record.field("Contained_Id").value().toInt();

    FoodCollection::ContainedTypes::ContainedType type =
      FoodCollection::ContainedTypes::fromHumanReadable(record.field("Contained_Type").value().toString());

    QSharedPointer<Food> containedFood;

    if (type == FoodCollection::ContainedTypes::SingleFood) {
      containedFood = SingleFood::getSingleFood(containedId);
    } else if (type == FoodCollection::ContainedTypes::CompositeFood) {
      containedFood = CompositeFood::getCompositeFood(containedId);
    } else {
      qDebug() << "Component has unknown type!";
    }

    if (!record.field(componentOrderField).value().isNull()) {
      order = record.field(componentOrderField).value().toInt();
    } else {
      qDebug() << "WARNING: Order field " << componentOrderField
               << " missing. Defaulting to sequential.";
      order++;
    }

    if (containedFood != NULL) {
      qDebug() << "Value of component ID field " << componentIdField << ": "
                << record.field(componentIdField).value().toInt();
      components.append
        (FoodComponent
          (containingCollection,
           record.field(componentIdField).value().toInt(),
           FoodAmount(containedFood, record.field("Magnitude").value().toDouble(),
                      Unit::getUnit(record.field("Unit").value().toString()),
                      record.field("Includes_Refuse").value().toBool()),
           order));
    } else {
      qDebug() << "Failed to create component!";
    }
  }

  return components;
}

void MySQLBackEnd::bindBaseAmount
  (const QSharedPointer<Food>& food, QSqlQuery& query,
   const QString& placeholder, Unit::Dimensions::Dimension dimension) const
{
  FoodAmount amount = food->getBaseAmount(dimension);

  if (amount.isDefined()) {
    query.bindValue(placeholder, amount.getAmount(Unit::getPreferredUnit(dimension)));
  } else {
    query.bindValue(placeholder, QVariant(QVariant::Double)); // Binds "NULL"
  }
}
