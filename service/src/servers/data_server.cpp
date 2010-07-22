#include "servers/data_server.h"
#include "servers/omissions.h"
#include <QStack>

DataLoadResponse DataLoadResponseObjects::serialize()
{
  DataLoadResponse resp;

  acquireDependentObjects();

  if (!omissions.groups && !group_objects.isEmpty()) {
    *(resp.mutable_grouploadresponse()) = group_objects.serialize();
  }

  if (!omissions.units && !unit_objects.isEmpty()) {
    *(resp.mutable_unitloadresponse()) = unit_objects.serialize();
  }

  if (!omissions.nutrients && !nutrient_objects.isEmpty()) {
    *(resp.mutable_nutrientloadresponse()) = nutrient_objects.serialize();
  }

  return resp;
}

void DataLoadResponseObjects::acquireDependentObjects()
{
  // Meals acquire Foods (singles, composites, templates)

  // Foods recursively acquire other Foods

  if (!(omissions.single_foods && omissions.composite_foods && omissions.templates))
  {
    QStack<QSharedPointer<Food> > acquisitionStack;
    QList<QSharedPointer<Food> > foods = food_objects.getFoods();
    FoodLoadResponseObjects new_food_objects;

    for (QList<QSharedPointer<Food> >::const_iterator i = foods.begin();
        i != foods.end(); ++i)
    {
      aquisitionStack.push(*i);
    }

    while (!acquisitionStack.isEmpty()) {

      QSharedPointer<Food> food = aquisitionStack.pop();

      if (!new_food_objects.contains(food)) {
        new_food_objects.addFood(food);

        QList<FoodComponent> components = food->getComponents();

        for (QList<FoodComponent>::const_iterator i = components.begin();
            i != components.end(); ++i)
        {
          QSharedPointer<Food> component_food = (*i)->getFoodAmount().getFood();

          if (!new_food_objects.contains(component_food)) {
            acquisitionStack.push(component_food);
          }
        }
      }
    }

    food_objects = new_food_objects;
  }

  // Foods acquire Nutrients

  // Nutrients and Specialized Units acquire Units

  if (!omissions.units) {
    if (!omissions.nutrients) {

      QList<QSharedPointer<const Nutrient> > nutrients = nutrient_objects.getNutrients();

      for (QList<QSharedPointer<const Nutrient> >::const_iterator i = nutrients.begin();
           i != nutrients.end(); ++i)
      {
        unit_objects.addUnit((*i)->getStandardUnit());
      }
    }
  }
}

namespace DataServer {

  DataLoadResponseObjects loadData(const DataLoadRequest& req)
  {
    DataLoadResponseObjects resp_objs((Omissions(req)));

    if (req.has_grouploadrequest()) {
      resp_objs.group_objects = GroupServer::loadGroups(req.grouploadrequest());
    }

    if (req.has_unitloadrequest()) {
      resp_objs.unit_objects = UnitServer::loadUnits(req.unitloadrequest());
    }

    if (req.has_nutrientloadrequest()) {
      resp_objs.nutrient_objects = NutrientServer::loadNutrients(req.nutrientloadrequest());
    }

    return resp_objs;
  }

}
