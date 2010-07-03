#include "mysql_back_end.h"
#include "libnutrition/data/data_cache.h"
#include "libnutrition/data/impl/composite_food_impl.h"
#include <QVariant>
#include <QDebug>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlRecord>
#include <QtSql/QSqlField>
#include <QtSql/QSqlError>
#include <stdexcept>

QSharedPointer<CompositeFood>  MySQLBackEnd::loadCompositeFood(int id)
{
  QSqlQuery query(db);

  query.prepare("SELECT composite_food.Composite_Id, composite_food.User_Id, "
                "       composite_food.Description, "
                "       composite_food.User_Id, composite_food.IsNonce, "
                "       composite_food.CreationDate, composite_food.ExpiryDate, "
                "       composite_food.Weight_g, composite_food.Volume_floz, "
                "       composite_food.Quantity, composite_food.Servings, "
                "       composite_food_link.CompositeLink_Id, composite_food_link.Contained_Type, "
                "       composite_food_link.Contained_Id, composite_food_link.Includes_Refuse,"
                "       composite_food_link.Magnitude, composite_food_link.Unit, "
                "       composite_food_link.IntrafoodOrder "
                "FROM"
                "        composite_food "
                "   LEFT JOIN composite_food_link "
                "        ON composite_food.Composite_Id = composite_food_link.Composite_Id "
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

QMultiMap<QString, int>  MySQLBackEnd::loadCompositeFoodNamesForUser
   (int userId, bool)
{
  QSqlQuery query(db);
  QMultiMap<QString, int> foods;

  // TODO: Use second argument includeExpired

  query.prepare("SELECT composite_food.Composite_Id, composite_food.Description,"
                 "       composite_food.CreationDate, composite_food.ExpiryDate "
                 "FROM   composite_food "
                 "WHERE  composite_food.User_Id = :userId");

  query.bindValue(":userId", userId);

  if (query.exec()) {
    while (query.next()) {
      const QSqlRecord& record = query.record();
      foods.insert(record.field("Description").value().toString() +
                   CompositeFood::generateExpirySuffix
                     (record.field("CreationDate").value().toDate(),
                      record.field("ExpiryDate").value().toDate()),
                   record.field("Composite_Id").value().toInt());
    }
  } else {
    qDebug() << "Query failed: " << query.lastError();
  }

  return foods;
}

void MySQLBackEnd::storeCompositeFood(const QSharedPointer<CompositeFood>& food)
{
  // TODO: Replace
  food->saveToDatabase();
}

QSharedPointer<CompositeFood>
  MySQLBackEnd::createCompositeFoodFromQueryResults
    (QSqlQuery& query)
{
  QSharedPointer<CompositeFoodImpl> food_impl;

  if (query.first()) {

    const QSqlRecord& record = query.record();
    int id = record.field("Composite_Id").value().toInt();

    food_impl = QSharedPointer<CompositeFoodImpl>
      (new CompositeFoodImpl(id,
                             record.field("Description").value().toString(),
                             record.field("User_Id").value().toInt(),
                             record.field("Weight_g").value().toDouble(),
                             record.field("Volume_floz").value().toDouble(),
                             record.field("Quantity").value().toDouble(),
                             record.field("Servings").value().toDouble(),
                             record.field("CreationDate").value().toDate(),
                             record.field("ExpiryDate").value().toDate(),
                             record.field("IsNonce").value().toBool()));

    DataCache<CompositeFood>::getInstance().insert(id, food_impl);
  }

  if (food_impl && query.isActive()) {
    query.seek(-1); // Reset to before first record
    food_impl->setComponents
      (createComponentsFromQueryResults
         (query, food_impl, "CompositeLink_Id", "IntrafoodOrder"));
    if (food_impl->needsToBeReSaved()) {
      food_impl->saveToDatabase();
    }
  }

  return food_impl;
}

