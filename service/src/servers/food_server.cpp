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

void FoodLoadResponseObjects::setError
  (BackEnd::FoodTypes::FoodType foodType, const QString& errorMessage)
{
  Error& err = subErrors[foodType];

  if (err.isError) {
    err.errorMessage = "Multiple errors occurred while processing this request.";
  } else {
    err.errorMessage = errorMessage;
  }

  err.isError = true;
}

void FoodLoadResponseObjects::setError(const QString& errorMessage)
{
  if (error.isError) {
    error.errorMessage = "Multiple errors occurred while processing this request.";
  } else {
    error.errorMessage = errorMessage;
  }

  error.isError = true;
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
  FoodData fdata = getFoodData();

  // TODO: See if there is a better way to do this copying

  if (!omissions.single_foods) {
    serializeSingleFoods(fdata, *(resp.mutable_singlefoodloadresponse()));
  }

  if (!omissions.composite_foods) {
    serializeCompositeFoods(fdata, *(resp.mutable_compositefoodloadresponse()));
  }

  if (!omissions.templates) {
    serializeTemplates(fdata, *(resp.mutable_templateloadresponse()));
  }

  assert(fdata.meals_size() == 0);

  setError(resp, error);

  return resp;
}

SingleFoodLoadResponse FoodLoadResponseObjects::serializeSingleFoods() const
{
  SingleFoodLoadResponse resp;
  return serializeSingleFoods(getFoodData(), resp);
}

SingleFoodLoadResponse& FoodLoadResponseObjects::serializeSingleFoods
  (const FoodData& fdata, SingleFoodLoadResponse& resp) const
{
  for (int i = 0; i < fdata.singlefoods_size(); ++i) {
    *(resp.add_singlefoods()) = fdata.singlefoods(i);
  }

  if (subErrors.contains(BackEnd::FoodTypes::SingleFood)) {
    setError(resp, subErrors[BackEnd::FoodTypes::SingleFood]);
  }

  return resp;
}

CompositeFoodLoadResponse FoodLoadResponseObjects::serializeCompositeFoods() const
{
  CompositeFoodLoadResponse resp;
  return serializeCompositeFoods(getFoodData(), resp);
}

CompositeFoodLoadResponse& FoodLoadResponseObjects::serializeCompositeFoods
  (const FoodData& fdata, CompositeFoodLoadResponse& resp) const
{
  for (int i = 0; i < fdata.compositefoods_size(); ++i) {
    *(resp.add_compositefoods()) = fdata.compositefoods(i);
  }

  if (subErrors.contains(BackEnd::FoodTypes::CompositeFood)) {
    setError(resp, subErrors[BackEnd::FoodTypes::CompositeFood]);
  }

  return resp;
}

TemplateLoadResponse FoodLoadResponseObjects::serializeTemplates() const
{
  TemplateLoadResponse resp;
  return serializeTemplates(getFoodData(), resp);
}

TemplateLoadResponse& FoodLoadResponseObjects::serializeTemplates
  (const FoodData& fdata, TemplateLoadResponse& resp) const
{
  for (int i = 0; i < fdata.templates_size(); ++i) {
    *(resp.add_templates()) = fdata.templates(i);
  }

  if (subErrors.contains(BackEnd::FoodTypes::Template)) {
    setError(resp, subErrors[BackEnd::FoodTypes::Template]);
  }

  return resp;
}

FoodData FoodLoadResponseObjects::getFoodData() const
{
  FoodData fdata;

  for (QList<QSharedPointer<const Food> >::const_iterator i = foods.begin();
       i != foods.end(); ++i)
  {
    (*i)->serialize(fdata);
  }

  return fdata;
}

template <typename T>
void FoodLoadResponseObjects::setError(T& resp, const Error& err)
{
  if (err.isError) {
    if (resp.has_error() && resp.error().iserror()) {
      resp.mutable_error()->set_errormessage("Multiple errors occurred while processing this request");
    } else {
      resp.mutable_error()->set_errormessage(err.errorMessage.toStdString());
    }
    resp.mutable_error()->set_iserror(true);
  }
}
