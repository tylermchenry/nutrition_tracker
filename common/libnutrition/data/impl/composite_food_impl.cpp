/*
 * composite_food.cpp
 *
 *  Created on: May 14, 2010
 *      Author: tmchenry
 */

#include "composite_food_impl.h"
#include "libnutrition/backend/back_end.h"
#include <QDebug>

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
  BackEnd::getBackEnd()->storeCompositeFood
    (getCanonicalSharedPointer().dynamicCast<CompositeFood>());
}

void CompositeFoodImpl::deleteFromDatabase()
{
  BackEnd::getBackEnd()->deleteCompositeFood
    (getCanonicalSharedPointer().dynamicCast<CompositeFood>());
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
