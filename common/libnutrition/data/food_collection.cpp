/*
 * food_collection.cpp
 * Part of nutrition_tracker
 *
 *  Created on: May 22, 2010
 *      Author: Tyler McHenry <tyler@nerdland.net>
 */

#include "food_collection.h"
#include "impl/food_collection_impl.h"
#include "libnutrition/backend/back_end.h"
#include "data_cache.h"
#include "single_food.h"
#include "composite_food.h"
#include "template.h"

int FoodCollection::nextCollectionId = 0;

QSharedPointer<FoodCollection> FoodCollection::createFoodCollection(const QString& name)
{
  QSharedPointer<FoodCollection> collection
    (new FoodCollectionImpl(nextCollectionId++, name, User::getLoggedInUser()->getId(),
                            QList<FoodComponent>(), 0, 0, 0, 1));
  DataCache<FoodCollection>::getInstance().insert
    (collection->getFoodCollectionId(), collection);
  return collection;
}

QSharedPointer<FoodCollection> FoodCollection::createFoodCollection
   (const QSharedPointer<FoodCollection>& copy)
{
  QSharedPointer<FoodCollection> collection;
  if (copy) {
    collection = createFoodCollection(copy->getName());
    collection->replaceWith(copy);
  }
  return collection;
}

QMultiMap<QString, QPair<FoodCollection::ContainedTypes::ContainedType, int> >
  FoodCollection::getFoodsForUser(int userId)
{
  QMultiMap<QString, QPair<BackEnd::FoodTypes::FoodType, int> > foodNames;
  QMultiMap<QString, QPair<ContainedTypes::ContainedType, int> > rv_foodNames;

  foodNames = BackEnd::getBackEnd()->loadFoodNamesForUser(userId, true);

  for (QMultiMap<QString, QPair<BackEnd::FoodTypes::FoodType, int> >::const_iterator i =
       foodNames.begin(); i != foodNames.end(); ++i)
  {
    // TODO: Get rid of ContainedType entirely in favor of BackEnd::FoodTypes

    ContainedTypes::ContainedType ctype =
      ContainedTypes::fromHumanReadable
        (BackEnd::FoodTypes::toContainedType(i.value().first));

    rv_foodNames.insert(i.key(), qMakePair(ctype, i.value().second));
  }

  return rv_foodNames;
}

QMultiMap<QString, QPair<FoodCollection::ContainedTypes::ContainedType, int> >
  FoodCollection::getFoodsForUser(int userId, ContainedTypes::ContainedType type)
{
  QMultiMap<QString, QPair<FoodCollection::ContainedTypes::ContainedType, int> > foods;
  QMultiMap<QString, int> rawFoods;

  if (type == ContainedTypes::SingleFood) {
    rawFoods = SingleFood::getFoodsForUser(userId);
  } else if (type == ContainedTypes::CompositeFood) {
    rawFoods = CompositeFood::getFoodsForUser(userId);
  } else if (type == ContainedTypes::Template) {
    rawFoods = Template::getFoodsForUser(userId);
  }

  for (QMultiMap<QString, int>::const_iterator i = rawFoods.begin();
       i != rawFoods.end(); ++i)
  {
    foods.insert(i.key(), QPair<ContainedTypes::ContainedType, int>
      (type, i.value()));
  }

  return foods;
}

QSharedPointer<Food> FoodCollection::getCanonicalSharedPointer() const
{
  return DataCache<FoodCollection>::getInstance().get(getFoodCollectionId());
}

FoodCollection::ContainedTypes::ContainedType FoodCollection::ContainedTypes::fromHumanReadable
  (const QString& str)
{
  QString lowerStr = str.toLower();

  if (lowerStr == "food")          return ContainedTypes::SingleFood;
  if (lowerStr == "compositefood") return ContainedTypes::CompositeFood;
  if (lowerStr == "template")      return ContainedTypes::Template;

  throw std::range_error("String does not describe a contained type.");
}

QString FoodCollection::ContainedTypes::toHumanReadable(ContainedType src)
{
  switch (src) {
    case SingleFood:    return "Food";
    case CompositeFood: return "CompositeFood";
    case Template:      return "Template";
    default:     throw std::range_error("ContainedType enumeration out of range.");
  }
}

