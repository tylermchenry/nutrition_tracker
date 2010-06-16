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
    (new FoodCollection(nextCollectionId++, name, QSet<FoodComponent>(),
                        0, 0, 0, 1));
  foodCollectionCache[collection->id] = collection;
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
  QMultiMap<QString, QPair<FoodCollection::ContainedTypes::ContainedType, int> > foods;
  QMultiMap<QString, int> singleFoods = SingleFood::getFoodsForUser(userId);
  QMultiMap<QString, int> compositeFoods = CompositeFood::getFoodsForUser(userId);

  for (QMultiMap<QString, int>::const_iterator i = singleFoods.begin();
       i != singleFoods.end(); ++i)
  {
    foods.insert(i.key(), QPair<ContainedTypes::ContainedType, int>
      (ContainedTypes::SingleFood, i.value()));
  }

  for (QMultiMap<QString, int>::const_iterator i = compositeFoods.begin();
       i != compositeFoods.end(); ++i)
  {
    foods.insert(i.key(), QPair<ContainedTypes::ContainedType, int>
      (ContainedTypes::CompositeFood, i.value()));
  }

  return foods;
}

FoodCollection::FoodCollection(const QString& id, const QString& name,
                             const QSet<FoodComponent>& components,
                             double weightAmount, double volumeAmount,
                             double quantityAmount, double servingAmount)
  : Food(id, name, weightAmount, volumeAmount,
         quantityAmount, servingAmount), id(-1), components(components),
         nextTemporaryId(-1)
{
}

FoodCollection::FoodCollection(const QString& id, const QString& name,
                             double weightAmount, double volumeAmount,
                             double quantityAmount, double servingAmount)
  : Food(id, name, weightAmount, volumeAmount,
         quantityAmount, servingAmount), id(-1), nextTemporaryId(-1)
{
}

FoodCollection::FoodCollection(int id, const QString& name,
                             const QSet<FoodComponent>& components,
                             double weightAmount, double volumeAmount,
                             double quantityAmount, double servingAmount)
  : Food("COLLECTION_" + QString::number(id), name, weightAmount, volumeAmount,
         quantityAmount, servingAmount), id(id), components(components),
         nextTemporaryId(-1)
{
}

FoodCollection::~FoodCollection()
{
}

QSet<FoodComponent> FoodCollection::getComponents() const
{
  return components;
}

QVector<FoodAmount> FoodCollection::getComponentAmounts() const
{
  QVector<FoodAmount> amounts;

  amounts.reserve(components.size());

  for (QSet<FoodComponent>::const_iterator i = components.begin(); i != components.end(); ++i)
  {
    amounts.push_back(i->getFoodAmount());
  }

  return amounts;
}

QMap<QString, NutrientAmount> FoodCollection::getNutrients() const
{
  // TODO: Intelligently cache instead of recomputing each time

  QMap<QString, NutrientAmount> nutrients;

  for (QSet<FoodComponent>::const_iterator i = components.begin(); i != components.end(); ++i)
  {
    mergeNutrients(nutrients, i->getFoodAmount().getScaledNutrients());
  }

  return nutrients;
}

NutrientAmount FoodCollection::getCaloriesFromNutrientId
   (const QString& nutrId) const
{
  // TODO: Intelligently cache instead of recomputing each time

  NutrientAmount totalCaloriesFromNutrient
    (Nutrient::getNutrientByName(Nutrient::CALORIES_NAME), 0);

  for (QSet<FoodComponent>::const_iterator i = components.begin(); i != components.end(); ++i)
  {
    NutrientAmount caloriesFromNutrient =
      i->getFoodAmount().getScaledCaloriesFromNutrientId(nutrId);

    if (caloriesFromNutrient.isDefined()) {
      totalCaloriesFromNutrient += caloriesFromNutrient;
    }
  }

  return totalCaloriesFromNutrient;
}

FoodComponent FoodCollection::addComponent(const FoodAmount& foodAmount)
{
  int order = 0;

  if (!components.empty()) {
    // TODO: Something more efficient?
    order = components.toList().last().getOrder()+1;
  }

  qDebug() << "Adding new (unsaved) component: " << foodAmount.getAmount()
           << foodAmount.getUnit()->getAbbreviation() << " of "
           << foodAmount.getFood()->getName()
           << " to collection: " << getName() << " [id = "
           << nextTemporaryId << "]";
  FoodComponent component(getCanonicalSharedPointerToCollection(),
                          nextTemporaryId--, foodAmount, order);
  components.insert(component);
  return component;
}

QVector<FoodComponent> FoodCollection::addComponents(const QVector<FoodAmount>& components)
{
  QVector<FoodComponent> addedComponents;

  for (QVector<FoodAmount>::const_iterator i = components.begin(); i != components.end(); ++i)
  {
    addedComponents.push_back(addComponent(*i));
  }

  return addedComponents;
}

FoodComponent FoodCollection::changeComponentAmount(const FoodComponent& component, const FoodAmount& amount)
{
  if (components.contains(component)) {
    if (!amount.isDefined() || component.getFoodAmount().getFood()->getId() != amount.getFood()->getId()) {
      throw std::logic_error("Attempted to change a component amount to another or an undefined food.");
    }
    FoodComponent newComponent(getCanonicalSharedPointerToCollection(), component.getId(), amount,
                               component.getOrder());
    components.remove(component);
    components.insert(newComponent);
    return newComponent;
  } else {
    qDebug() << "Component with ID " << component.getId() << " is not part of "
             << " this collection (id " << id << "). Claims to be part of collection"
             << component.getContainingCollection()->getId();

    throw std::logic_error("Attempted to change a component not part of this collection.");
  }
}

void FoodCollection::removeComponent(const FoodComponent& component)
{
  if (components.contains(component)) {
    if (component.getId() >= 0) {
      removedIds.insert(component.getId());
    }
    components.remove(component);
  } else if (newIds.contains(component.getId())) {
    removeComponent
      (FoodComponent(getCanonicalSharedPointerToCollection(),
                     newIds[component.getId()], component.getFoodAmount(), component.getOrder()));
    newIds.remove(component.getId());
  }
}

void FoodCollection::clearComponents()
{
  for (QSet<FoodComponent>::const_iterator i = components.begin(); i != components.end(); ++i)
  {
    if (i->getId() >= 0) {
      removedIds.insert(i->getId());
    }
  }

  components.clear();
  newIds.clear();
}

QVector<FoodComponent> FoodCollection::merge(const QSharedPointer<const FoodCollection>& otherFC)
{
  if (otherFC != NULL) {
    return addComponents(otherFC->getComponentAmounts());
  } else {
    return QVector<FoodComponent>();
  }
}

void FoodCollection::replaceWith(const QSharedPointer<const FoodCollection>& otherFC)
{
  // This is kind of like an assignment operator, but not really. It replaces
  // the components of this object with components from the other object, but
  // the component objects "rebased" to have this object as their containing
  // collection. It also copies over the newIds and removedIds data from the
  // old class. It's not an assignment operator because it only operates on
  // the collection level. The ID of this collection is not modified, and
  // derived classes may have futher information that is not overwritten by
  // this call.

  if (otherFC == NULL) return;

  components.clear();

  for (QSet<FoodComponent>::const_iterator i = otherFC->components.begin(); i != otherFC->components.end(); ++i)
  {
    components.insert(FoodComponent(getCanonicalSharedPointerToCollection(),
                                    i->getId(), i->getFoodAmount(), i->getOrder()));
  }

  newIds = otherFC->newIds;
  removedIds = otherFC->removedIds;
  nextTemporaryId = otherFC->nextTemporaryId;
}


void FoodCollection::saveToDatabase()
{
  throw std::logic_error("Attempted to save a bare food collection to the database.");
}

void FoodCollection::setComponents(const QSet<FoodComponent>& components)
{
  qDebug() << "Setting components of " << this->getName() << " to a set of "
           << components.size() << " components";
  clearComponents();
  this->components = components;
}

void FoodCollection::replaceComponent(const FoodComponent& oldComponent,
                                           const FoodComponent& newComponent)
{
  if (oldComponent.getId() >= 0) {
    throw std::logic_error("Attempted to replace a non-temporary component.");
  }

  if (newComponent.getId() < 0) {
    throw std::logic_error("Attempted to replace a component with a temporary component.");
  }

  qDebug() << "Replacing component ID " << oldComponent.getId()
           << " with new component ID " << newComponent.getId();

  newIds.insert(oldComponent.getId(), newComponent.getId());

  components.remove(oldComponent);
  components.insert(newComponent);
}

QSharedPointer<Food> FoodCollection::getCanonicalSharedPointer()
{
  return foodCollectionCache[id].toStrongRef();
}

QSharedPointer<const Food> FoodCollection::getCanonicalSharedPointer() const
{
  return foodCollectionCache[id].toStrongRef();
}

QSharedPointer<FoodCollection> FoodCollection::getCanonicalSharedPointerToCollection()
{
  return getCanonicalSharedPointer().dynamicCast<FoodCollection>();
}

QSharedPointer<const FoodCollection>
  FoodCollection::getCanonicalSharedPointerToCollection() const
{
  return getCanonicalSharedPointer().dynamicCast<const FoodCollection>();
}

QSet<FoodComponent> FoodCollection::createComponentsFromQueryResults
  (QSqlQuery& query, const QSharedPointer<FoodCollection>& containingCollection,
   const QString& componentIdField, const QString& componentOrderField)
{
  QSet<FoodComponent> components;

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
    } else {
      qDebug() << "Component has unknown type!";
    }

    int order = 0;

    if (!record.field(componentOrderField).value().isNull()) {
      order = record.field(componentOrderField).value().toInt();
    }

    if (containedFood != NULL) {
      qDebug() << "Value of component ID field " << componentIdField << ": "
                << record.field(componentIdField).value().toInt();
      components.insert
        (FoodComponent
          (containingCollection,
           record.field(componentIdField).value().toInt(),
           FoodAmount(containedFood, record.field("Magnitude").value().toDouble(),
                      Unit::createUnitFromRecord(record)),
           order));
    } else {
      qDebug() << "Failed to create component!";
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

