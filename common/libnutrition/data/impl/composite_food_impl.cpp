/*
 * composite_food.cpp
 *
 *  Created on: May 14, 2010
 *      Author: tmchenry
 */

#include "composite_food_impl.h"
#include "libnutrition/proto/data/data.pb.h"
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

FoodData& CompositeFood::serialize(FoodData& fdata) const
{
  *(fdata.add_compositefoods()) = serialize();
  return fdata;
}

CompositeFoodData CompositeFoodImpl::serialize() const
{
  CompositeFoodData cfdata;

  cfdata.set_id(id);
  cfdata.set_name(getName().toStdString());
  cfdata.set_ownerid(getOwnerId());

  cfdata.set_weightamount(getBaseAmount(Unit::Dimensions::Weight).getAmount());
  cfdata.set_volumeamount(getBaseAmount(Unit::Dimensions::Volume).getAmount());
  cfdata.set_quantityamount(getBaseAmount(Unit::Dimensions::Quantity).getAmount());
  cfdata.set_servingamount(getBaseAmount(Unit::Dimensions::Serving).getAmount());

  cfdata.set_creationdate_iso8601(creationDate.toString(Qt::ISODate).toStdString());
  cfdata.set_expirydate_iso8601(expiryDate.toString(Qt::ISODate).toStdString());

  cfdata.set_isnonce(isNonce());

  for (QMap<int, FoodComponent>::const_iterator i = getRawComponents().begin();
       i != getRawComponents().end(); ++i)
  {
    *(cfdata.add_components()) = i.value().serialize();
  }

  return cfdata;
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
