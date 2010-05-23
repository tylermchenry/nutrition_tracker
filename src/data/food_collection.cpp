/*
 * food_collection.cpp
 * Part of nutrition_tracker
 *
 *  Created on: May 22, 2010
 *      Author: Tyler McHenry <tyler@nerdland.net>
 */

#include "food_collection.h"
#include "single_food.h"
#include "composite_food.h"
#include <QVariant>
#include <QDebug>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlRecord>
#include <QtSql/QSqlField>
#include <QtSql/QSqlError>

int FoodCollection::nextCollectionId = 0;

QMap<int, QWeakPointer<FoodCollection> > FoodCollection::foodCollectionCache;

QSharedPointer<FoodCollection> FoodCollection::createFoodCollection(const QString& name)
{
  QSharedPointer<FoodCollection> collection
    (new FoodCollection(nextCollectionId++, name, QVector<FoodAmount>(),
                        0, 0, 0, 1));
  foodCollectionCache[collection->id] = collection;
  return collection;
}

FoodCollection::FoodCollection(const QString& id, const QString& name,
                             const QVector<FoodAmount>& components,
                             double weightAmount, double volumeAmount,
                             double quantityAmount, double servingAmount)
  : Food(id, name, weightAmount, volumeAmount,
         quantityAmount, servingAmount), id(-1), components(components)
{
}

FoodCollection::FoodCollection(int id, const QString& name,
                             const QVector<FoodAmount>& components,
                             double weightAmount, double volumeAmount,
                             double quantityAmount, double servingAmount)
  : Food("COLLECTION_" + QString::number(id), name, weightAmount, volumeAmount,
         quantityAmount, servingAmount), id(id), components(components)
{
}

FoodCollection::~FoodCollection()
{
}

QVector<FoodAmount> FoodCollection::getComponents() const
{
  return components;
}

QMap<QString, NutrientAmount> FoodCollection::getNutrients() const
{
  // TODO: Intelligently cache instead of recomputing each time

  QMap<QString, NutrientAmount> nutrients;

  for (QVector<FoodAmount>::const_iterator i = components.begin();
       i != components.end(); ++i)
  {
    mergeNutrients(nutrients, i->getScaledNutrients());
  }

  return nutrients;
}

void FoodCollection::addComponent(const FoodAmount& foodAmount)
{
  components.push_back(foodAmount);
}

void FoodCollection::addComponents(const QVector<FoodAmount>& components)
{
  this->components += components;
}

void FoodCollection::clearComponents()
{
  components.clear();
}

void FoodCollection::saveToDatabase()
{
  throw std::logic_error("Attempted to save a bare food collection to the database.");
}

QSharedPointer<Food> FoodCollection::getCanonicalSharedPointer()
{
  return foodCollectionCache[id].toStrongRef();
}

QSharedPointer<const Food> FoodCollection::getCanonicalSharedPointer() const
{
  return foodCollectionCache[id].toStrongRef();
}

QVector<FoodAmount> FoodCollection::createComponentsFromQueryResults
  (QSqlQuery& query)
{
  QVector<FoodAmount> components;

  while (query.next()) {

    const QSqlRecord& record = query.record();

    int containedId = record.field("Contained_Id").value().toInt();

    ContainedTypes::ContainedType type =
      ContainedTypes::fromHumanReadable(record.field("Contained_Type").value().toString());

    QSharedPointer<const Food> containedFood;

    if (type == ContainedTypes::SingleFood) {
      containedFood = SingleFood::getSingleFood(containedId);
    } else if (type == ContainedTypes::CompositeFood) {
      containedFood = CompositeFood::getCompositeFood(containedId);
    }

    if (containedFood != NULL) {
      components.push_back(FoodAmount
        (containedFood, record.field("Magnitude").value().toDouble(),
         Unit::createUnitFromRecord(record)));
    }
  }

  return components;
}

QMap<QString, NutrientAmount>& FoodCollection::mergeNutrients
  (QMap<QString, NutrientAmount>& nutrients,
   const QMap<QString, NutrientAmount>& newNutrients)
{
  for (QMap<QString, NutrientAmount>::const_iterator i = newNutrients.begin();
       i != newNutrients.end(); ++i)
  {
    nutrients[i.key()] += i.value();
  }

  return nutrients;
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

