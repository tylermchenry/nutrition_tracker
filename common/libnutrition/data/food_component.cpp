/*
 * food_component.cpp
 * Part of nutrition_tracker
 *
 *  Created on: May 25, 2010
 *      Author: Tyler McHenry <tyler@nerdland.net>
 */

#include "food_component.h"
#include "food_collection.h"
#include "food.h"
#include "single_food.h"
#include "composite_food.h"
#include "libnutrition/proto/data/data.pb.h"
#include <QSet>
#include <QDebug>

FoodComponent::FoodComponent()
  : id(-1), order(0)
{
}

FoodComponent::FoodComponent(const QSharedPointer<FoodCollection>& containingCollection,
                                 int id, const FoodAmount& foodAmount, int order)
  : containingCollection(containingCollection), id(id), foodAmount(foodAmount), order(order)
{
  qDebug() << "Created a component with ID = " << id << " order = " << order;
  if (foodAmount.isDefined()) {
    qDebug() << "Component is " << foodAmount.getAmount() << " "
             << foodAmount.getUnit()->getAbbreviation() << " of "
             << foodAmount.getFood()->getName();
  }
}

FoodComponent::~FoodComponent()
{
}

FoodComponentData FoodComponent::serialize() const
{
  FoodComponentData fcdata;

  fcdata.set_id(id);

  if (QSharedPointer<const SingleFood> singleFood =
      foodAmount.getFood().dynamicCast<const SingleFood>())
  {
    fcdata.set_foodid(singleFood->getSingleFoodId());
    fcdata.set_foodtype(FoodComponentData::SingleFood);
  }
  else if (QSharedPointer<const CompositeFood> compositeFood =
            foodAmount.getFood().dynamicCast<const CompositeFood>())
  {
    fcdata.set_foodid(compositeFood->getCompositeFoodId());
    fcdata.set_foodtype(FoodComponentData::CompositeFood);
  }

  fcdata.set_amount(foodAmount.getAmount());
  fcdata.set_unitabbreviation(foodAmount.getUnit()->getAbbreviation().toStdString());
  fcdata.set_includesrefuse(foodAmount.includesRefuse());
  fcdata.set_order(order);

  return fcdata;
}

bool FoodComponent::operator< (const FoodComponent& rhs) const
{
  qDebug() << "Comparing two components.";
  if (containingCollection) {
    if (containingCollection.data() == rhs.containingCollection.data()) {
      if (foodAmount.isDefined()) {
        qDebug() << "Comparing by order: " << order << " < " << rhs.order;
        return (order < rhs.order);
      } else {
        return rhs.foodAmount.isDefined();
      }
    } else {
      return (containingCollection.data() < rhs.containingCollection.data());
    }
  } else {
    return rhs.containingCollection.isNull();
  }
}

bool FoodComponent::operator== (const FoodComponent& rhs) const
{
  return containingCollection && rhs.containingCollection &&
     containingCollection.data() == rhs.containingCollection.data() &&
     foodAmount.isDefined() && rhs.foodAmount.isDefined() &&
     foodAmount.getFood().data() == rhs.foodAmount.getFood().data() && id == rhs.id;
}

uint qHash(const FoodComponent& component)
{
  uint hash = qHash(component.getContainingCollection().data()) +
    3 * qHash(component.getId()) + 5 * qHash(component.getOrder());

  if (component.getFoodAmount().isDefined()) {
    hash += 7 * qHash(component.getFoodAmount().getFood().data());
  }

  return hash;
}
