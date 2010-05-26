/*
 * food_component.cpp
 * Part of nutrition_tracker
 *
 *  Created on: May 25, 2010
 *      Author: Tyler McHenry <tyler@nerdland.net>
 */

#include "food_component.h"
#include "food.h"
#include <QSet>
#include <QDebug>

FoodComponent::FoodComponent()
  : id(-1), order(0)
{
}

FoodComponent::FoodComponent(int id, const FoodAmount& foodAmount, int order)
  : id(id), foodAmount(foodAmount), order(order)
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

bool FoodComponent::operator< (const FoodComponent& rhs) const
{
  if (foodAmount.isDefined()) {
    if (foodAmount.getFood().data() == rhs.foodAmount.getFood().data()) {
      return (order < rhs.order);
    } else {
      return (foodAmount.getFood().data() < rhs.foodAmount.getFood().data());
    }
  } else {
    return rhs.foodAmount.isDefined();
  }
}

bool FoodComponent::operator== (const FoodComponent& rhs) const
{
  return foodAmount.isDefined() && rhs.foodAmount.isDefined() &&
     foodAmount.getFood().data() == rhs.foodAmount.getFood().data() && id == rhs.id;
}

uint qHash(const FoodComponent& component)
{
  uint hash = qHash(component.getId()) + 3 * qHash(component.getOrder());

  if (component.getFoodAmount().isDefined()) {
    hash += 5 * qHash(component.getFoodAmount().getFood().data());
  }

  return hash;
}
