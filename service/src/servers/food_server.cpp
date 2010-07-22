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
(const QSharedPointer<const Food>& food)
{
  if (food && !foodIds.contains(food->getId())) {
    foodIds.insert(food->getId());
    foods.append(food);
  }
}

void FoodLoadResponseObjects::addFoods
  (const QVector<QSharedPointer<const Food> >& foods)
{
  for (QVector<QSharedPointer<const Food> >::const_iterator i = foods.begin();
       i != foods.end(); ++i)
  {
    addFood(*i);
  }
}

void FoodLoadResponseObjects::addFoods
  (const QList<QSharedPointer<const Food> >& foods)
{
  for (QList<QSharedPointer<const Food> >::const_iterator i = foods.begin();
       i != foods.end(); ++i)
  {
    addFood(*i);
  }
}

void FoodLoadResponseObjects::clear()
{
  foods.clear();
  foodIds.clear();
}

void FoodLoadResponseObjects::replaceFoods(const QList<QSharedPointer<const Food> >& foods)
{
  clear();
  addFoods(foods);
}

bool FoodLoadResponseObjects::contains(const QSharedPointer<const Food>& food) const
{
  return (food && foodIds.contains(food->getId()));
}

QList<QSharedPointer<const Food> > FoodLoadResponseObjects::getFoods() const
{
  return foods;
}

QSet<QString> FoodLoadResponseObjects::getFoodIds() const
{
  return foodIds;
}

DataLoadResponse& FoodLoadResponseObjects::serialize
  (DataLoadResponse& resp, const Omissions& omissions) const
{
  FoodData fdata;

  for (QList<QSharedPointer<const Food> >::const_iterator i = foods.begin();
       i != foods.end(); ++i)
  {
    (*i)->serialize(fdata);
  }

  // TODO: See if there is a better way to do this copying

  if (!omissions.single_foods) {
    for (int i = 0; i < fdata.singlefoods_size(); ++i) {
      *(resp.mutable_singlefoodloadresponse()->add_singlefoods()) =
        fdata.singlefoods(i);
    }
  }

  if (!omissions.composite_foods) {
    for (int i = 0; i < fdata.compositefoods_size(); ++i) {
      *(resp.mutable_compositefoodloadresponse()->add_compositefoods()) =
        fdata.compositefoods(i);
    }
  }

  if (!omissions.templates) {
    for (int i = 0; i < fdata.templates_size(); ++i) {
      *(resp.mutable_templateloadresponse()->add_templates()) =
        fdata.templates(i);
    }
  }

  assert(fdata.meals_size() == 0);

  return resp;
}
