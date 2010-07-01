/*
 * composite_food.cpp
 *
 *  Created on: May 14, 2010
 *      Author: tmchenry
 */

#include "composite_food_impl.h"
#include "libnutrition/data/nutrient_amount.h"
#include "libnutrition/data/food_amount.h"
#include "libnutrition/data/single_food.h"
#include <QVariant>
#include <QDebug>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlRecord>
#include <QtSql/QSqlField>
#include <QtSql/QSqlError>

int CompositeFoodImpl::tempId = -1;

CompositeFoodImpl::CompositeFoodImpl
  (int id, const QString& name, int ownerId,
   const QList<FoodComponent>& components,
   double weightAmount, double volumeAmount,
   double quantityAmount, double servingAmount,
 QDate creationDate, QDate expiryDate, bool nonce)
  : FoodCollectionImpl
      ("COMPOSITE_" + QString::number(id), name, ownerId, components,
       weightAmount, volumeAmount, quantityAmount, servingAmount),
    id(id), nonce(nonce), creationDate(creationDate), expiryDate(expiryDate)
{
  validateExpiryDate();
  if (needsToBeReSaved()) {
    saveToDatabase();
  }
}

CompositeFoodImpl::CompositeFoodImpl
  (int id, const QString& name, int ownerId,
   double weightAmount, double volumeAmount,
   double quantityAmount, double servingAmount,
   QDate creationDate, QDate expiryDate, bool nonce)
  : FoodCollectionImpl
      ("COMPOSITE_" + QString::number(id), name, ownerId,
       weightAmount, volumeAmount, quantityAmount, servingAmount),
    id(id), nonce(nonce), creationDate(creationDate), expiryDate(expiryDate)
{
  validateExpiryDate();
}

CompositeFoodImpl::CompositeFoodImpl
  (const QSharedPointer<const CompositeFood>& copy)
  : FoodCollectionImpl("COMPOSITE_" + QString::number(tempId), copy),
    id(tempId--),
    nonce(copy ? copy->isNonce() : false),
    creationDate(copy ? copy->getCreationDate() : QDate::currentDate()),
    expiryDate(copy ? copy->getExpiryDate() : QDate())
{
  qDebug() << "Created new composite food with temporary ID " << id;
  validateExpiryDate();
}

CompositeFoodImpl::~CompositeFoodImpl()
{
}

QString CompositeFoodImpl::getDisplayName() const
{
  QString name = getName() + generateExpirySuffix(creationDate, expiryDate);

  if (nonce) {
    return "[" + name + "]";
  } else {
    return name;
  }
}

void CompositeFoodImpl::setCreationDate(const QDate& date)
{
  creationDate = date;
  validateExpiryDate();
}

void CompositeFoodImpl::setExpiryDate(const QDate& date)
{
  expiryDate = date;
  validateExpiryDate();
}

void CompositeFoodImpl::saveToDatabase()
{
  QSqlDatabase db = QSqlDatabase::database("nutrition_db");
  QSqlQuery query(db);

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

  query.bindValue(":id", (id >= 0 ? QVariant(id) : QVariant(QVariant::Int)));

  query.bindValue(":user_id", getOwnerId());
  query.bindValue(":user_id2", getOwnerId());
  query.bindValue(":name", getName());
  query.bindValue(":name2", getName());

  query.bindValue(":nonce", nonce);
  query.bindValue(":nonce2", nonce);

  query.bindValue(":creation", creationDate.isNull() ? QVariant(QVariant::Date) : creationDate);
  query.bindValue(":creation2", creationDate.isNull() ? QVariant(QVariant::Date) : creationDate);
  query.bindValue(":expiry", expiryDate.isNull() ? QVariant(QVariant::Date) : expiryDate);
  query.bindValue(":expiry2", expiryDate.isNull() ? QVariant(QVariant::Date) : expiryDate);

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
    id = assignNewId(query.lastInsertId().toInt());
  }

  QSet<int> removedLinkIds = getRemovedIds();

  for (QSet<int>::const_iterator i = removedLinkIds.begin(); i != removedLinkIds.end(); ++i)
  {
    query.prepare("DELETE FROM composite_food_link WHERE CompositeLink_Id=:linkId");

    query.bindValue(":linkId", *i);

    if (!query.exec()) {
      qDebug() << "Failed to delete removed composite food item: " << query.lastError();
      return;
    }
  }

  deleteRemovedNonceFoods();

  QList<FoodComponent> components = getComponents();
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
        replaceComponent
        (*i, FoodComponent(getCanonicalSharedPointerToCollection(),
                           newId, i->getFoodAmount(), i->getOrder()));
      }
    }
  }
}

void CompositeFoodImpl::deleteFromDatabase()
{
  if (id < 0) return;

  QSqlDatabase db = QSqlDatabase::database("nutrition_db");
  QSqlQuery query(db);

  query.prepare("DELETE FROM composite_food_link WHERE Composite_Id=:compositeId");
  query.bindValue(":compositeId", id);

  if (!query.exec()) {
    qDebug() << "Failed to delete components of " << getName() << ": "
              << query.lastError();
    return;
  }

  query.prepare("DELETE FROM composite_food WHERE Composite_Id=:compositeId");
  query.bindValue(":compositeId", id);

  if (!query.exec()) {
    qDebug() << "Failed to composite food " << getName() << ": "
              << query.lastError();
    return;
  }

  clearComponents();
  deleteRemovedNonceFoods();
  removeFromCache();
}

QSharedPointer<Food> CompositeFoodImpl::cloneNonce() const
{
  if (nonce) {
    return createNewNonceCompositeFood(getBaseAmount());
  } else {
    return QSharedPointer<Food>();
  }
}

void CompositeFoodImpl::validateExpiryDate()
{
  if (!creationDate.isNull() && !expiryDate.isNull() && expiryDate < creationDate) {
    expiryDate = creationDate;
  }
}
