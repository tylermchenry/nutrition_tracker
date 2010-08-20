/*
 * update_components.cpp
 *
 *  Created on: Aug 17, 2010
 *      Author: tyler
 */

#include "update_components.h"
#include <libnutrition/data/single_food.h>
#include <libnutrition/data/composite_food.h>
#include <QList>
#include <QMap>

namespace UpdateComponents {

  ComponentModifications::ComponentModifications()
    : bIsError(false)
  {
  }

  void ComponentModifications::setError(const QString& errorMessage)
  {
    if (bIsError) {
      this->errorMessage = "Multiple errors occurred while updating components.";
    } else {
      this->errorMessage = errorMessage;
    }

    bIsError = true;
  }

  bool ComponentModifications::isError() const
  {
    return bIsError;
  }

  QString ComponentModifications::getErrorMessage() const
  {
    return errorMessage;
  }

  QMap<int, FoodComponent> createReceivedComponents
    (const QSharedPointer<FoodCollection>& collection,
     const google::protobuf::RepeatedPtrField<FoodComponentData>& pb_components,
     QList<QString>& errors)
  {
    QMap<int, FoodComponent> componentsById;

    for (int i = 0; i < pb_components.size(); ++i) {
      const FoodComponentData& fcData = pb_components.Get(i);

      if (fcData.has_foodtype() && fcData.has_foodid() && fcData.has_amount() &&
          fcData.has_unitabbreviation() && fcData.has_id() && fcData.has_order())
      {
        FoodCollection::ContainedTypes::ContainedType type;
        QSharedPointer<Food> food;

        type = static_cast<FoodCollection::ContainedTypes::ContainedType>
          (fcData.foodtype());

        if (type == FoodCollection::ContainedTypes::SingleFood) {
          food = SingleFood::getSingleFood(fcData.foodid());
        } else if (type == FoodCollection::ContainedTypes::CompositeFood) {
          food = CompositeFood::getCompositeFood(fcData.foodid());
        } else {
          errors.append("A component food type was invalid.");
          continue;
        }

        QSharedPointer<const Unit> unit = Unit::getUnit
          (QString::fromStdString(fcData.unitabbreviation()));

        if (!food || !unit) {
          errors.append("A component food or unit ID was invalid.");
          continue;
        }

        FoodAmount amount(food, fcData.amount(), unit, fcData.includesrefuse());

        componentsById[fcData.id()] =
          (FoodComponent(collection, fcData.id(), amount, fcData.order()));
      }
      else
      {
        errors.append("A component had incomplete data.");
      }
    }

    return componentsById;
  }

  QMap<int, FoodComponent> getExistingComponents
    (const QSharedPointer<FoodCollection>& collection)
  {
    QList<FoodComponent> components = collection->getComponents();
    QMap<int, FoodComponent> componentsById;

    for (QList<FoodComponent>::const_iterator i = components.begin();
         i != components.end(); ++i)
    {
      componentsById[i->getId()] = *i;
    }

    return componentsById;
  }

  void deleteRemovedComponents
    (const QSharedPointer<FoodCollection>& collection,
     const QList<int> idsToKeep,
     QMap<int, FoodComponent>& existingComponentsById,
     QList<QString>& /* errors */)
  {
    QList<FoodComponent> componentsToRemove;

    for (QMap<int, FoodComponent>::const_iterator i = existingComponentsById.begin();
         i != existingComponentsById.end(); ++i)
    {
      if(!idsToKeep.contains(i.key())) {
        componentsToRemove.append(i.value());
      }
    }

    for (QList<FoodComponent>::const_iterator i = componentsToRemove.begin();
         i != componentsToRemove.end(); ++i)
    {
      collection->removeComponent(*i);
      existingComponentsById.remove(i->getId());
    }
  }

  QMap<int, int> addNewComponents
    (const QSharedPointer<FoodCollection>& collection,
     const QMap<int, FoodComponent>& receivedComponentsById,
     QMap<int, FoodComponent>& existingComponentsById,
     QList<QString>& errors)
   {
    QMap<int, int> idAssignments;

    for (QMap<int, FoodComponent>::const_iterator i = receivedComponentsById.begin();
         i != receivedComponentsById.end(); ++i)
    {
      if(!existingComponentsById.contains(i.key())) {
        if (i.key() < 0) {
          FoodComponent component =
            collection->addComponent(i.value().getFoodAmount());
          idAssignments[i.key()] = component.getId();
          existingComponentsById[component.getId()] = component;
        } else {
          errors.append("A new component had a non-temporary ID.");
        }
      }
    }

    return idAssignments;
   }

  void updateOrderingAndAmounts
    (const QSharedPointer<FoodCollection>& collection,
     const QMap<int, FoodComponent>& receivedComponentsById,
     const QMap<int, int>& idAssignments,
     QMap<int, FoodComponent>& existingComponentsById,
     QList<QString>& /* errors */)
  {
    for (QMap<int, FoodComponent>::const_iterator i = receivedComponentsById.begin();
         i != receivedComponentsById.end(); ++i)
    {
      int realId = i.key();

      if (idAssignments.contains(i.key())) {
        realId = idAssignments[i.key()];
      }

      if (i.value().getOrder() != existingComponentsById[realId].getOrder()) {

        QList<FoodComponent> reorderedComponents =
          collection->changeComponentOrder
          (existingComponentsById[realId], i.value().getOrder());

        for (QList<FoodComponent>::const_iterator j = reorderedComponents.begin();
        j != reorderedComponents.end(); ++j)
        {
          existingComponentsById[j->getId()] = *j;
        }
      }

      if (i.value().getFoodAmount() != existingComponentsById[realId].getFoodAmount())
      {
        existingComponentsById[realId] = collection->changeComponentAmount
        (existingComponentsById[realId], i.value().getFoodAmount());
      }
    }
  }

  ComponentModifications recordModifications
    (const QMap<int, FoodComponent>& receivedComponentsById,
     const QMap<int, int>& idAssignments,
     const QMap<int, FoodComponent>& existingComponentsById,
     const QList<QString>& errors)
  {
    ComponentModifications modifications;

    // Modifications idAssignments array is backwards from internal idAssignments

    for (QMap<int, int>::const_iterator i = idAssignments.begin();
         i != idAssignments.end(); ++i)
    {
      modifications.idAssignments[i.value()] = i.key();
    }

    for (QMap<int, FoodComponent>::const_iterator i = receivedComponentsById.begin();
         i != receivedComponentsById.end(); ++i)
    {
      int realId = i.key();

      if (idAssignments.contains(i.key())) {
        realId = idAssignments[i.key()];
      }

      if (i.value().getOrder() != existingComponentsById[realId].getOrder()) {
        modifications.orderChanges[realId] =
          qMakePair(i.value().getOrder(), existingComponentsById[realId].getOrder());
      }
    }

    for (QList<QString>::const_iterator i = errors.begin(); i != errors.end(); ++i)
    {
      modifications.setError(*i);
    }

    return modifications;
  }

  ComponentModifications updateComponents
    (const QSharedPointer<FoodCollection>& collection,
     const google::protobuf::RepeatedPtrField<FoodComponentData>& pb_components)
  {
    QList<QString> errors;

    QMap<int, FoodComponent> existingComponentsById =
      getExistingComponents(collection);

    const QMap<int, FoodComponent> receivedComponentsById =
      createReceivedComponents(collection, pb_components, errors);

    deleteRemovedComponents
      (collection, receivedComponentsById.keys(), existingComponentsById, errors);

    const QMap<int, int> idAssignments = addNewComponents
      (collection, receivedComponentsById, existingComponentsById, errors);

    updateOrderingAndAmounts
      (collection, receivedComponentsById, idAssignments,
       existingComponentsById, errors);

    return recordModifications
      (receivedComponentsById, idAssignments, existingComponentsById, errors);
  }

  ComponentModifications updateComponentModifications
    (const QSharedPointer<FoodCollection>& collection,
     const ComponentModifications& cmods)
  {
    ComponentModifications new_cmods;

    if (cmods.isError()) {
      new_cmods.setError(cmods.getErrorMessage());
    }

    for (QMap<int, int>::const_iterator i = cmods.idAssignments.begin();
         i != cmods.idAssignments.end(); ++i)
    {
      FoodComponent component = collection->getComponent(i.key());
      if (component.getContainingCollection() == collection) {
        new_cmods.idAssignments[component.getId()] = i.value();
      }
    }

    for (QMap<int, QPair<int, int> >::const_iterator i = cmods.orderChanges.begin();
         i != cmods.orderChanges.end(); ++i)
    {
      FoodComponent component = collection->getComponent(i.key());
      if (component.getContainingCollection() == collection) {
        new_cmods.orderChanges[component.getId()] = i.value();
      }
    }

    return new_cmods;
  }
}

