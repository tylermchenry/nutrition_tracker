/*
 * food_collection.cpp
 * Part of nutrition_tracker
 *
 *  Created on: May 22, 2010
 *      Author: Tyler McHenry <tyler@nerdland.net>
 */

#include "food_collection_impl.h"
#include "libnutrition/data/single_food.h"
#include "libnutrition/data/composite_food.h"
#include "libnutrition/data/template.h"
#include <QVariant>
#include <QDebug>

FoodCollectionImpl::FoodCollectionImpl
  (const QString& id, const QString& name, int ownerId,
   const QList<FoodComponent>& components,
   double weightAmount, double volumeAmount,
   double quantityAmount, double servingAmount)
  : FoodImpl(id, name, ownerId, weightAmount, volumeAmount,
             quantityAmount, servingAmount), id(-1), nextTemporaryId(-1),
             bNeedsToBeReSaved(false)
{
  setComponents(components);
}

FoodCollectionImpl::FoodCollectionImpl
  (const QString& id, const QString& name, int ownerId,
   double weightAmount, double volumeAmount,
   double quantityAmount, double servingAmount)
  : FoodImpl(id, name, ownerId, weightAmount, volumeAmount,
             quantityAmount, servingAmount), id(-1), nextTemporaryId(-1),
             bNeedsToBeReSaved(false)
{
}

FoodCollectionImpl::FoodCollectionImpl
  (int id, const QString& name, int ownerId,
   const QList<FoodComponent>& components,
   double weightAmount, double volumeAmount,
   double quantityAmount, double servingAmount)
  : FoodImpl("COLLECTION_" + QString::number(id), name, ownerId, weightAmount,
             volumeAmount, quantityAmount, servingAmount), id(id),
             nextTemporaryId(-1), bNeedsToBeReSaved(false)
{
  setComponents(components);
}

FoodCollectionImpl::FoodCollectionImpl
  (const QString& id, const QSharedPointer<const FoodCollection>& copy)
  : FoodImpl(id, copy), id(-1),
    nextTemporaryId(copy ? copy->getNextTemporaryId() : -1),
    bNeedsToBeReSaved(copy ? copy->needsToBeReSaved() : false)
{
  if (copy) {
    setComponents(copy->getComponents());
  }
}

FoodCollectionImpl::~FoodCollectionImpl()
{
}

QList<FoodComponent> FoodCollectionImpl::getComponents() const
{
  return components.values();
}

QVector<FoodAmount> FoodCollectionImpl::getComponentAmounts() const
{
  QVector<FoodAmount> amounts;

  amounts.reserve(components.size());

  for (QMap<int, FoodComponent>::const_iterator i = components.begin(); i != components.end(); ++i)
  {
    amounts.push_back(i.value().getFoodAmount());
  }

  return amounts;
}

QMap<QString, NutrientAmount> FoodCollectionImpl::getNutrients() const
{
  // TODO: Intelligently cache instead of recomputing each time

  QMap<QString, NutrientAmount> nutrients;

  for (QMap<int, FoodComponent>::const_iterator i = components.begin(); i != components.end(); ++i)
  {
    mergeNutrients(nutrients, i.value().getFoodAmount().getScaledNutrients());
  }

  return nutrients;
}

NutrientAmount FoodCollectionImpl::getCaloriesFromNutrientId
   (const QString& nutrId) const
{
  // TODO: Intelligently cache instead of recomputing each time

  NutrientAmount totalCaloriesFromNutrient
    (Nutrient::getNutrientByName(Nutrient::CALORIES_NAME), 0);

  for (QMap<int, FoodComponent>::const_iterator i = components.begin(); i != components.end(); ++i)
  {
    NutrientAmount caloriesFromNutrient =
      i.value().getFoodAmount().getScaledCaloriesFromNutrientId(nutrId);

    if (caloriesFromNutrient.isDefined()) {
      totalCaloriesFromNutrient += caloriesFromNutrient;
    }
  }

  return totalCaloriesFromNutrient;
}

FoodComponent FoodCollectionImpl::addComponent(const FoodAmount& foodAmount)
{
  int order = 0;

  if (!components.empty()) {
    // TODO: Something more efficient?
    order = components.keys().last()+1;
  }


  qDebug() << "Adding new (unsaved) component: " << foodAmount.getAmount()
           << foodAmount.getUnit()->getAbbreviation() << " of "
           << foodAmount.getFood()->getName()
           << " to collection: " << getName() << " [id = "
           << nextTemporaryId << "]";

  FoodComponent component;

  if (foodAmount.getFood()->isNonce()) {
    qDebug() << "Component is a nonce. Making a copy.";
    component = FoodComponent(getCanonicalSharedPointerToCollection(),
                              nextTemporaryId--,
                              FoodAmount(foodAmount.getFood()->cloneNonce(),
                                         foodAmount.getAmount(), foodAmount.getUnit()),
                              component.getOrder());
  } else {
    component = FoodComponent(getCanonicalSharedPointerToCollection(),
                              nextTemporaryId--, foodAmount, order);
  }

  components.insert(order, component);
  return component;
}

QVector<FoodComponent> FoodCollectionImpl::addComponents(const QVector<FoodAmount>& components)
{
  QVector<FoodComponent> addedComponents;

  for (QVector<FoodAmount>::const_iterator i = components.begin(); i != components.end(); ++i)
  {
    addedComponents.push_back(addComponent(*i));
  }

  return addedComponents;
}

FoodComponent FoodCollectionImpl::getComponent(int componentId)
{
  if (newIds.contains(componentId)) {
    return getComponent(newIds[componentId]);
  } else {
    for (QMap<int, FoodComponent>::const_iterator i = components.begin();
         i != components.end(); ++i)
    {
      if (i.value().getId() == componentId) {
        return i.value();
      }
    }
  }

  return FoodComponent();
}

FoodComponent FoodCollectionImpl::changeComponentAmount(const FoodComponent& component, const FoodAmount& amount)
{
  if (hasComponent(component)) {
    if (!amount.isDefined() || component.getFoodAmount().getFood()->getId() != amount.getFood()->getId()) {
      throw std::logic_error("Attempted to change a component amount to another or an undefined food.");
    }
    FoodComponent newComponent(getCanonicalSharedPointerToCollection(), component.getId(), amount,
                               component.getOrder());
    components[component.getOrder()] = newComponent;
    return newComponent;
  } else {
    qDebug() << "Component with ID " << component.getId() << " is not part of "
             << " this collection (id " << getId() << "). Claims to be part of collection"
             << component.getContainingCollection()->getId();

    throw std::logic_error("Attempted to change a component not part of this collection.");
  }
}

void FoodCollectionImpl::removeComponent(const FoodComponent& component)
{
  if (hasComponent(component)) {
    if (component.getId() >= 0) {
      removedIds.insert(component.getId());
      if (component.getFoodAmount().getFood()->isNonce()) {
        removedNonceFoods.append(component.getFoodAmount().getFood());
      }
    }
    components.remove(component.getOrder());
  } else if (newIds.contains(component.getId())) {
    removeComponent
      (FoodComponent(getCanonicalSharedPointerToCollection(),
                     newIds[component.getId()], component.getFoodAmount(), component.getOrder()));
    newIds.remove(component.getId());
  }
}

QList<FoodComponent> FoodCollectionImpl::changeComponentOrder
  (const FoodComponent& component, int order)
{
  // Slides everything down one, if necessary
  QList<FoodComponent> reorderedComponents;

  if (hasComponent(component)) {

    components.remove(component.getOrder());

    if (components.contains(order)) {
      reorderedComponents = changeComponentOrder(components[order], order+1);
    }

    components[order] =
      FoodComponent(getCanonicalSharedPointerToCollection(), component.getId(),
                    component.getFoodAmount(), order);

    reorderedComponents.append(components[order]);
  }

  return reorderedComponents;
}

void FoodCollectionImpl::clearComponents()
{
  for (QMap<int, FoodComponent>::const_iterator i = components.begin(); i != components.end(); ++i)
  {
    if (i.value().getId() >= 0) {
      removedIds.insert(i.value().getId());

      if (i.value().getFoodAmount().getFood()->isNonce()) {
        removedNonceFoods.append(i.value().getFoodAmount().getFood());
      }
    }
  }

  components.clear();
  newIds.clear();
}

QVector<FoodComponent> FoodCollectionImpl::merge(const QSharedPointer<const FoodCollection>& otherFC)
{
  if (otherFC != NULL) {
    return addComponents(otherFC->getComponentAmounts());
  } else {
    return QVector<FoodComponent>();
  }
}

void FoodCollectionImpl::replaceWith(const QSharedPointer<const FoodCollection>& otherFC)
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

  for (QMap<int, FoodComponent>::const_iterator i = otherFC->getRawComponents().begin();
       i != otherFC->getRawComponents().end(); ++i)
  {
    const FoodComponent& c = i.value();
    Q_ASSERT(i.key() == c.getOrder());
    Q_ASSERT(!components.contains(i.key()));
    components.insert
      (i.key(), FoodComponent(getCanonicalSharedPointerToCollection(),
                              c.getId(), c.getFoodAmount(), c.getOrder()));
  }

  newIds = otherFC->getNewIds();
  removedIds = otherFC->getRemovedIds();
  nextTemporaryId = otherFC->getNextTemporaryId();
}


void FoodCollectionImpl::saveToDatabase()
{
  throw std::logic_error("Attempted to save a bare food collection to the database.");
}

void FoodCollectionImpl::deleteFromDatabase()
{
  throw std::logic_error("Attempted to delete a bare food collection from the database.");
}

FoodData& FoodCollectionImpl::serialize(FoodData&) const
{
  throw std::logic_error("Attempted to serialize a bare food collection.");
}

QSharedPointer<FoodCollection>
  FoodCollectionImpl::getCanonicalSharedPointerToCollection() const
{
  return getCanonicalSharedPointer().dynamicCast<FoodCollection>();
}

void FoodCollectionImpl::setComponents(const QList<FoodComponent>& components)
{
  qDebug() << "Setting components of " << this->getName() << " to a set of "
           << components.size() << " components";
  clearComponents();
  addComponents(components);
}

void FoodCollectionImpl::replaceComponent(const FoodComponent& oldComponent,
                                           const FoodComponent& newComponent)
{
  if (oldComponent.getId() >= 0) {
    throw std::logic_error("Attempted to replace a non-temporary component.");
  }

  if (newComponent.getId() < 0) {
    throw std::logic_error("Attempted to replace a component with a temporary component.");
  }

  if (!hasComponent(oldComponent)) {
    throw std::logic_error("Attempted to replace a non-existent component");
  }

  if (oldComponent.getOrder() != newComponent.getOrder()) {
    throw std::logic_error("Mismatched ordering when attempting to replace a component.");
  }

  qDebug() << "Replacing component ID " << oldComponent.getId()
           << " with new component ID " << newComponent.getId();

  newIds.insert(oldComponent.getId(), newComponent.getId());
  components[oldComponent.getOrder()] = newComponent;
}

void FoodCollectionImpl::deleteRemovedNonceFoods()
{
  for (QList<QSharedPointer<Food> >::const_iterator i = removedNonceFoods.begin();
       i != removedNonceFoods.end(); ++i)
  {
    (*i)->deleteFromDatabase();
  }

  removedNonceFoods.clear();
}

void FoodCollectionImpl::addComponents(const QList<FoodComponent>& newComponents)
{
  int maxOrder = -1;

  if (!components.empty()) {
    maxOrder = components.keys().last();
  }

  for (QList<FoodComponent>::const_iterator i = newComponents.begin(); i != newComponents.end(); ++i)
  {
    qDebug() << "Inserting component: " << i->getFoodAmount().getAmount()
             << i->getFoodAmount().getUnit()->getAbbreviation()
             << " of " << i->getFoodAmount().getFood()->getName()
             << " order = " << i->getOrder();

    FoodComponent c = *i;

    if (c.getContainingCollection() != getCanonicalSharedPointerToCollection()) {

      qDebug() << "Component is from another collection; Rebasing with temporary ID " << nextTemporaryId;
      c = FoodComponent(getCanonicalSharedPointerToCollection(),
                        nextTemporaryId--, c.getFoodAmount(), c.getOrder());

      if (c.getFoodAmount().getFood()->isNonce()) {
        qDebug() << "Component is a nonce. Making a copy.";
        const FoodAmount& amt = c.getFoodAmount();
        c = FoodComponent(getCanonicalSharedPointerToCollection(),
                          nextTemporaryId--,
                          FoodAmount(amt.getFood()->cloneNonce(), amt.getAmount(), amt.getUnit()),
                          c.getOrder());
      }
    }

    int order = c.getOrder();

    if (components.contains(order)) {

      qDebug() << "Order " << order << " is already in use. Replacing with order "
                << maxOrder+1;

      order = ++maxOrder;
      components.insert(order, FoodComponent(getCanonicalSharedPointerToCollection(),
                                             c.getId(), c.getFoodAmount(), order));
      bNeedsToBeReSaved = true;
    } else {
      qDebug() << "Order " << c.getOrder() << " is OK.";
      components.insert(c.getOrder(), c);
    }

    maxOrder = std::max(maxOrder, order);
  }
}

QMap<QString, NutrientAmount>& FoodCollectionImpl::mergeNutrients
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


