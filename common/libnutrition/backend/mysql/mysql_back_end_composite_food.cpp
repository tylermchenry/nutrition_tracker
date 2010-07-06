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
  if (!food) {
    throw std::logic_error("Attempted to store a NULL food");
  }

  QSqlQuery query(db);

  QSharedPointer<CompositeFoodImpl> food_impl =
    food.dynamicCast<CompositeFoodImpl>();

  qDebug() << "Saving composite food to database.";

  // This needs to work either for a new food or an update to an existing food

  query.prepare("INSERT INTO composite_food "
                "  (Composite_Id, User_Id, Description, IsNonce, "
                "   CreationDate, ExpiryDate, Weight_g, Volume_floz, "
                "   Quantity, Servings) "
                "VALUES "
                "  (:id, :user_id, :name, :nonce, :creation, :expiry,"
                "   :weight, :volume, :quantity, :servings) "
                "ON DUPLICATE KEY UPDATE"
                "  User_Id=:user_id2, Description=:name2, IsNonce=:nonce2, "
                "  CreationDate=:creation2, ExpiryDate=:expiry2, "
                "  Weight_g=:weight2, Volume_floz=:volume2, Quantity=:quantity2, Servings=:servings2");

  query.bindValue(":id", (food->getCompositeFoodId() >= 0 ?
    QVariant(food->getCompositeFoodId()) : QVariant(QVariant::Int)));

  query.bindValue(":user_id", food->getOwnerId());
  query.bindValue(":user_id2", food->getOwnerId());
  query.bindValue(":name", food->getName());
  query.bindValue(":name2", food->getName());

  query.bindValue(":nonce", food->isNonce());
  query.bindValue(":nonce2", food->isNonce());

  QDate creationDate = food->getCreationDate();
  QDate expiryDate = food->getExpiryDate();

  query.bindValue(":creation", creationDate.isNull() ? QVariant(QVariant::Date) : creationDate);
  query.bindValue(":creation2", creationDate.isNull() ? QVariant(QVariant::Date) : creationDate);
  query.bindValue(":expiry", expiryDate.isNull() ? QVariant(QVariant::Date) : expiryDate);
  query.bindValue(":expiry2", expiryDate.isNull() ? QVariant(QVariant::Date) : expiryDate);

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
    throw std::runtime_error("Failed to save composite food to database.");
  }

  if (food->getCompositeFoodId() < 0) {
    int newId = query.lastInsertId().toInt();
    DataCache<CompositeFood>::getInstance().changeKey(food->getCompositeFoodId(), newId);
    qDebug() << "Assigned real ID " << newId
              << " to composite food with temp ID " << food->getCompositeFoodId();
    food_impl->setCompositeFoodId(newId);
  }

  QSet<int> removedLinkIds = food_impl->getRemovedIds();

  for (QSet<int>::const_iterator i = removedLinkIds.begin(); i != removedLinkIds.end(); ++i)
  {
    query.prepare("DELETE FROM composite_food_link WHERE CompositeLink_Id=:linkId");

    query.bindValue(":linkId", *i);

    if (!query.exec()) {
      qDebug() << "Failed to delete removed composite food item: " << query.lastError();
      return;
    }
  }

  food_impl->deleteRemovedNonceFoods();

  QList<FoodComponent> components = food->getComponents();
  for (QList<FoodComponent>::const_iterator i = components.begin(); i != components.end(); ++i)
  {
    if (i->getFoodAmount().getFood()->isNonce()) {
      i->getFoodAmount().getFood()->saveToDatabase();
    }

    if (!query.prepare("INSERT INTO composite_food_link "
        "  (CompositeLink_Id, Composite_Id, Contained_Type, "
        "   Contained_Id, Includes_Refuse, Magnitude, Unit, IntrafoodOrder) "
        "VALUES "
        "  (:linkId, :compositeId, :containedType, "
        "   :containedId, :includesRefuse, :magnitude, :unit, :order) "
        "ON DUPLICATE KEY UPDATE "
        "  Includes_Refuse=:includesRefuse2, Magnitude=:magnitude2, "
        "  Unit=:unit2, IntrafoodOrder=:order2")) {
      qDebug() << "Failed to prepare query: " << query.lastError();
    }

    query.bindValue(":linkId", i->getId() >= 0 ? QVariant(i->getId()) : QVariant());

    query.bindValue(":compositeId", food->getCompositeFoodId());

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

    query.bindValue(":includesRefuse", i->getFoodAmount().includesRefuse());
    query.bindValue(":magnitude", i->getFoodAmount().getAmount());
    query.bindValue(":unit", i->getFoodAmount().getUnit()->getAbbreviation());
    query.bindValue(":order", i->getOrder());

    query.bindValue(":includesRefuse2", i->getFoodAmount().includesRefuse());
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
        food_impl->replaceComponent
          (*i, FoodComponent(food_impl->getCanonicalSharedPointerToCollection(),
                             newId, i->getFoodAmount(), i->getOrder()));
      }
    }
  }
}

void MySQLBackEnd::deleteCompositeFood(const QSharedPointer<CompositeFood>& food)
{
  if (!food) {
    throw std::logic_error("Attempted to delete a NULL food");
  } else if (food->getCompositeFoodId() < 0) {
    // Silently ignore requests to delete temporaries; it's simpler to do that
    // than to force the client to check whether a food is temporary or not
    // every time it wants to delete something.
    return;
  }

  QSharedPointer<CompositeFoodImpl> food_impl =
    food.dynamicCast<CompositeFoodImpl>();

  QSqlQuery query(db);

  query.prepare("DELETE FROM composite_food_link WHERE Composite_Id=:compositeId");
  query.bindValue(":compositeId", food->getCompositeFoodId());

  if (!query.exec()) {
    qDebug() << "Failed to delete components of " << food->getName() << ": "
              << query.lastError();
    return;
  }

  query.prepare("DELETE FROM composite_food WHERE Composite_Id=:compositeId");
  query.bindValue(":compositeId", food->getCompositeFoodId());

  if (!query.exec()) {
    qDebug() << "Failed to composite food " << food->getName() << ": "
              << query.lastError();
    return;
  }

  food_impl->clearComponents();
  food_impl->deleteRemovedNonceFoods();

  DataCache<CompositeFood>::getInstance().remove(food->getCompositeFoodId());
}

void MySQLBackEnd::deleteCompositeFood(int id)
{
  deleteCompositeFood(CompositeFood::getCompositeFood(id));
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

