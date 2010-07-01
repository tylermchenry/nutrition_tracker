/*
 * composite_food.cpp
 *
 *  Created on: May 14, 2010
 *      Author: tmchenry
 */

#include "composite_food.h"
#include "impl/composite_food_impl.h"
#include "nutrient_amount.h"
#include "food_amount.h"
#include "single_food.h"
#include <QVariant>
#include <QDebug>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlRecord>
#include <QtSql/QSqlField>
#include <QtSql/QSqlError>

QMap<int, QWeakPointer<CompositeFood> > CompositeFood::compositeFoodCache;

QSharedPointer<CompositeFood> CompositeFood::createNewCompositeFood
  (const QSharedPointer<const CompositeFood>& copy)
{
  QSharedPointer<CompositeFood> food(new CompositeFoodImpl(copy));
  compositeFoodCache[food->getCompositeFoodId()] = food;
  return food;
}

QSharedPointer<CompositeFood> CompositeFood::createNewNonceCompositeFood
  (const FoodAmount& basisAmount)
{
  QSharedPointer<CompositeFood> food = createNewCompositeFood();

  food->setNonce(true);

  if (basisAmount.isDefined()) {

    food->setName(basisAmount.getFood()->getName());

    QList<FoodComponent> components = basisAmount.getFood()->getComponents();

    if (components.empty()) {
      food->addComponent(basisAmount);
    } else {
      for (QList<FoodComponent>::iterator i = components.begin(); i != components.end(); ++i) {
        food->addComponent(i->getFoodAmount());
      }
    }

    food->setBaseAmount(basisAmount.getAmount(), basisAmount.getUnit());
  }

  return food;
}

QSharedPointer<CompositeFood> CompositeFood::getCompositeFood(int id)
{
  QSqlDatabase db = QSqlDatabase::database("nutrition_db");
  QSqlQuery query(db);

  if (compositeFoodCache[id]) {
    return compositeFoodCache[id].toStrongRef();
  }

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

QSharedPointer<CompositeFood> CompositeFood::createCompositeFoodFromQueryResults
  (QSqlQuery& query)
{
  QSharedPointer<CompositeFood> food;

  if (query.first()) {

    const QSqlRecord& record = query.record();
    int id = record.field("Composite_Id").value().toInt();

    if (!compositeFoodCache[id]) {

      food = QSharedPointer<CompositeFood>
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

      compositeFoodCache[id] = food;

    } else {
      return compositeFoodCache[id].toStrongRef();
    }

  }

  if (food && query.isActive()) {
    query.seek(-1); // Reset to before first record
    food->setComponents
      (createComponentsFromQueryResults
         (query, food, "CompositeLink_Id", "IntrafoodOrder"));
    if (food->needsToBeReSaved()) {
      food->saveToDatabase();
    }
  }

  return food;
}

QMultiMap<QString, int> CompositeFood::getFoodsForUser(int userId)
{
  QSqlDatabase db = QSqlDatabase::database("nutrition_db");
  QSqlQuery query(db);
  QMultiMap<QString, int> foods;

  query.prepare("SELECT composite_food.Composite_Id, composite_food.Description,"
                 "       composite_food.CreationDate, composite_food.ExpiryDate "
                 "FROM   composite_food "
                 "WHERE  composite_food.User_Id = :userId");

  query.bindValue(":userId", userId);

  if (query.exec()) {
    while (query.next()) {
      const QSqlRecord& record = query.record();
      foods.insert(record.field("Description").value().toString() +
                   generateExpirySuffix(record.field("CreationDate").value().toDate(),
                                        record.field("ExpiryDate").value().toDate()),
                   record.field("Composite_Id").value().toInt());
    }
  } else {
    qDebug() << "Query failed: " << query.lastError();
  }

  return foods;
}

QString CompositeFood::generateExpirySuffix
  (const QDate& creation, const QDate& expiry)
{
  QString suffix;

  if (!expiry.isNull()) {
    if (!creation.isNull()) {
      if (expiry.year() == creation.year()) {
        if (expiry.month() == creation.month()) {
          if (expiry.day() == creation.day()) {
            suffix += " (" + expiry.toString("dd MMM yyyy") + ")";
          } else {
            suffix += " (" + creation.toString("dd") + " - " +
            expiry.toString("dd MMM yyyy") + ")";
          }
        } else {
          suffix += " (" + creation.toString("dd MMM") + " - " +
          expiry.toString("dd MMM yyyy") + ")";
        }
      } else {
        suffix += " (" + creation.toString("dd MMM yyyy") + " - " +
        expiry.toString("dd MMM yyyy") + ")";
      }
    } else {
      suffix += " (? - " + expiry.toString("dd MMM yyyy") + ")";
    }
  }

  return suffix;
}

QSharedPointer<Food> CompositeFood::getCanonicalSharedPointer() const
{
  return compositeFoodCache[getCompositeFoodId()].toStrongRef();
}

int CompositeFood::assignNewId(int newId)
{
  int oldId = getCompositeFoodId();
  if (newId != oldId) {
    compositeFoodCache[newId] = compositeFoodCache[oldId];
    compositeFoodCache.remove(oldId);
    qDebug() << "Assigned real ID " << newId
              << " to composite food with temp ID " << oldId;
  }
  return newId;
}

void CompositeFood::removeFromCache()
{
  compositeFoodCache.remove(getCompositeFoodId());
}
