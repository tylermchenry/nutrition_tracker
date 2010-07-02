/*
 * food_collection.cpp
 * Part of nutrition_tracker
 *
 *  Created on: May 22, 2010
 *      Author: Tyler McHenry <tyler@nerdland.net>
 */

#include "food_collection.h"
#include "impl/food_collection_impl.h"
#include "data_cache.h"
#include "single_food.h"
#include "composite_food.h"
#include "template.h"
#include <QVariant>
#include <QDebug>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlRecord>
#include <QtSql/QSqlField>
#include <QtSql/QSqlError>

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
  return getFoodsForUser(userId, ContainedTypes::SingleFood) +
    getFoodsForUser(userId, ContainedTypes::CompositeFood) +
    getFoodsForUser(userId, ContainedTypes::Template);
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

QSharedPointer<FoodCollection> FoodCollection::getCanonicalSharedPointerToCollection() const
{
  return getCanonicalSharedPointer().dynamicCast<FoodCollection>();
}

QList<FoodComponent> FoodCollection::createComponentsFromQueryResults
  (QSqlQuery& query, const QSharedPointer<FoodCollection>& containingCollection,
   const QString& componentIdField, const QString& componentOrderField)
{
  QList<FoodComponent> components;
  int order = -1;

  while (query.next()) {

    const QSqlRecord& record = query.record();

    if (record.field(componentIdField).value().isNull()) continue;

    int containedId = record.field("Contained_Id").value().toInt();

    ContainedTypes::ContainedType type =
      ContainedTypes::fromHumanReadable(record.field("Contained_Type").value().toString());

    QSharedPointer<Food> containedFood;

    if (type == ContainedTypes::SingleFood) {
      containedFood = SingleFood::getSingleFood(containedId);
    } else if (type == ContainedTypes::CompositeFood) {
      containedFood = CompositeFood::getCompositeFood(containedId);
    } else {
      qDebug() << "Component has unknown type!";
    }

    if (!record.field(componentOrderField).value().isNull()) {
      order = record.field(componentOrderField).value().toInt();
    } else {
      qDebug() << "WARNING: Order field " << componentOrderField
               << " missing. Defaulting to sequential.";
      order++;
    }

    if (containedFood != NULL) {
      qDebug() << "Value of component ID field " << componentIdField << ": "
                << record.field(componentIdField).value().toInt();
      components.append
        (FoodComponent
          (containingCollection,
           record.field(componentIdField).value().toInt(),
           FoodAmount(containedFood, record.field("Magnitude").value().toDouble(),
                      Unit::getUnit(record.field("Unit").value().toString()),
                      record.field("Includes_Refuse").value().toBool()),
           order));
    } else {
      qDebug() << "Failed to create component!";
    }
  }

  return components;
}

FoodCollection::ContainedTypes::ContainedType FoodCollection::ContainedTypes::fromHumanReadable
  (const QString& str)
{
  QString lowerStr = str.toLower();

  if (lowerStr == "food")          return ContainedTypes::SingleFood;
  if (lowerStr == "compositefood") return ContainedTypes::CompositeFood;

  throw std::range_error("String does not describe a contained type.");
}

QString FoodCollection::ContainedTypes::toHumanReadable(ContainedType src)
{
  switch (src) {
    case SingleFood:    return "Food";
    case CompositeFood: return "CompositeFood";
    default:     throw std::range_error("ContainedType enumeration out of range.");
  }
}

