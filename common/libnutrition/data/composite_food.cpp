/*
 * composite_food.cpp
 *
 *  Created on: May 14, 2010
 *      Author: tmchenry
 */

#include "composite_food.h"
#include "impl/composite_food_impl.h"
#include "data_cache.h"
#include "libnutrition/backend/back_end.h"
#include "nutrient_amount.h"
#include "food_amount.h"
#include "single_food.h"
#include <QDebug>

QSharedPointer<CompositeFood> CompositeFood::createNewCompositeFood
  (const QSharedPointer<const CompositeFood>& copy)
{
  QSharedPointer<CompositeFood> food(new CompositeFoodImpl(copy));
  DataCache<CompositeFood>::getInstance().insert(food->getCompositeFoodId(), food);
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
  if (DataCache<CompositeFood>::getInstance().contains(id)) {
    return DataCache<CompositeFood>::getInstance().get(id);
  } else {
    return BackEnd::getBackEnd()->loadCompositeFood(id);
  }
}

QMultiMap<QString, int> CompositeFood::getFoodsForUser(int userId)
{
  return BackEnd::getBackEnd()->loadCompositeFoodNamesForUser(userId);
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
  return DataCache<CompositeFood>::getInstance().get(getCompositeFoodId());
}

