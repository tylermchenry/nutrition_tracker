/*
 * food_server.cpp
 *
 *  Created on: Jul 21, 2010
 *      Author: tyler
 */

#include "servers/food_server.h"
#include "servers/data_server.h"
#include "libnutrition/data/food.h"
#include "libnutrition/proto/data/data.pb.h"
#include <cassert>

void FoodLoadResponseObjects::addFood
(const QSharedPointer<Food>& food)
{
  if (food && !foodIds.contains(food->getId())) {
    foodIds.insert(food->getId());
    foods.append(food);
  }
}

void FoodLoadResponseObjects::addFoods
  (const QVector<QSharedPointer<Food> >& foods)
{
  for (QVector<QSharedPointer<Food> >::const_iterator i = foods.begin();
       i != foods.end(); ++i)
  {
    addFood(*i);
  }
}

void FoodLoadResponseObjects::addFoods
  (const QList<QSharedPointer<Food> >& foods)
{
  for (QList<QSharedPointer<Food> >::const_iterator i = foods.begin();
       i != foods.end(); ++i)
  {
    addFood(*i);
  }
}

bool FoodLoadResponseObjects::contains(const QSharedPointer<Food>& food) const
{
  return (food && foodIds.contains(food->getId()));
}

QList<QSharedPointer<Food> > FoodLoadResponseObjects::getFoods() const
{
  return foods;
}

DataLoadResponse& FoodLoadResponseObjects::serialize(DataLoadResponse& resp) const
{
  FoodData fdata;

  for (QList<QSharedPointer<Food> >::const_iterator i = foods.begin();
       i != foods.end(); ++i)
  {
    (*i)->serialize(fdata);
  }

  // TODO: See if there is a better way to do this copying

  assert(fdata.meals_size() == 0);

  return resp;
}
