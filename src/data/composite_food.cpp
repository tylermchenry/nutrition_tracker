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

int CompositeFood::tempId = -1;

QSharedPointer<CompositeFood> CompositeFood::createNewCompositeFood()
{
  QSharedPointer<CompositeFood> food(new CompositeFood());
  compositeFoodCache[food->getCompositeFoodId()] = food;
  return food;
}

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

CompositeFood::CompositeFood()
  : FoodCollection("COMPOSITE_" + QString::number(tempId), "", 0, 0, 0, 0),
    id(tempId--)
{
  qDebug() << "Created new composite food with temporary ID " << id;
}

CompositeFood::~CompositeFood()
{
}

void CompositeFood::saveToDatabase()
{
  QSqlDatabase db = QSqlDatabase::database("nutrition_db");
  QSqlQuery query(db);

  qDebug() << "Saving composite food to database.";

  // This needs to work either for a new food or an update to an existing food

  query.prepare("INSERT INTO composite_food "
                "  (Composite_Id, User_Id, Description, Weight_g, Volume_floz, Quantity, Servings) "
                "VALUES "
                "  (:id, :user_id, :name, :weight, :volume, :quantity, :servings) "
                "ON DUPLICATE KEY UPDATE"
                "  User_Id=:user_id2, Description=:name2,"
                "  Weight_g=:weight2, Volume_floz=:volume2, Quantity=:quantity2, Servings=:servings2");

  query.bindValue(":id", (id >= 0 ? QVariant(id) : QVariant(QVariant::Int)));

  // TOOD: Real user IDs when users are implemented
  query.bindValue(":user_id", 1);
  query.bindValue(":user_id2", 1);
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
    throw std::runtime_error("Failed to save composite food to database.");
  }

  if (id < 0) {
    int newId = query.lastInsertId().toInt();
    compositeFoodCache[newId] = compositeFoodCache[id];
    compositeFoodCache.remove(id);
    qDebug() << "Assigned real ID " << newId << " to composite food with temp ID " << id;
    id = newId;
  }

  QSet<int> removedLinkIds = getRemovedIds();

  for (QSet<int>::const_iterator i = removedLinkIds.begin(); i != removedLinkIds.end(); ++i)
  {
    query.prepare("DELETE FROM composite_link WHERE CompositeLink_Id=:linkId");

    query.bindValue(":linkId", *i);

    if (!query.exec()) {
      qDebug() << "Failed to delete removed composite food item: " << query.lastError();
      return;
    }
  }

  QSet<FoodComponent> components = getComponents();
  for (QSet<FoodComponent>::const_iterator i = components.begin(); i != components.end(); ++i)
  {
    if (!query.prepare("INSERT INTO composite_food_link "
        "  (CompositeLink_Id, Composite_Id, Contained_Type, "
        "   Contained_Id, Magnitude, Unit, IntrafoodOrder) "
        "VALUES "
        "  (:linkId, :compositeId, :containedType, "
        "   :containedId, :magnitude, :unit, :order) "
        "ON DUPLICATE KEY UPDATE "
        "  Magnitude=:magnitude2, Unit=:unit2, IntrafoodOrder=:order2")) {
      qDebug() << "Failed to prepare query: " << query.lastError();
    }

    query.bindValue(":linkId", i->getId() >= 0 ? QVariant(i->getId()) : QVariant());

    query.bindValue(":compositeId", id);

    if (!i->getFoodAmount().isDefined()) continue;

    QSharedPointer<const Food> food = i->getFoodAmount().getFood();

    QSharedPointer<const SingleFood> singleFood;
    QSharedPointer<const CompositeFood> compositeFood;

    FoodCollection::ContainedTypes::ContainedType containedType;
    int containedId;

    if ((singleFood = food.dynamicCast<const SingleFood>()) != NULL) {
      containedType = FoodCollection::ContainedTypes::SingleFood;
      containedId = singleFood->getSingleFoodId();
    } else if ((compositeFood = food.dynamicCast<const CompositeFood>()) != NULL) {
      containedType = FoodCollection::ContainedTypes::CompositeFood;
      containedId = compositeFood->getCompositeFoodId();
    } else {
      continue;
    }

    query.bindValue(":containedType", FoodCollection::ContainedTypes::toHumanReadable(containedType));
    query.bindValue(":containedId", containedId);

    query.bindValue(":magnitude", i->getFoodAmount().getAmount());
    query.bindValue(":unit", i->getFoodAmount().getUnit()->getAbbreviation());
    query.bindValue(":order", i->getOrder());

    query.bindValue(":magnitude2", i->getFoodAmount().getAmount());
    query.bindValue(":unit2", i->getFoodAmount().getUnit()->getAbbreviation());
    query.bindValue(":order2", i->getOrder());

    if (!query.exec()) {
      qDebug() << "Failed to save " << food->getName() << " to meal: " << query.lastError();
    } else {
      if (i->getId() < 0) {
        int newId = query.lastInsertId().toInt();
        qDebug() << "Assigned real ID " << newId
                  << " to food component with temp ID " << i->getId();
        replaceComponent
        (*i, FoodComponent(getCanonicalSharedPointerToCollection(),
                           newId, i->getFoodAmount(), i->getOrder()));
      }
    }
  }
}

QSharedPointer<Food> CompositeFood::getCanonicalSharedPointer()
{
  return compositeFoodCache[id].toStrongRef();
}

QSharedPointer<const Food> CompositeFood::getCanonicalSharedPointer() const
{
  return compositeFoodCache[id].toStrongRef();
}

