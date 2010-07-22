#include "servers/data_server.h"
#include "servers/omissions.h"
#include <QStack>
#include <QQueue>

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

  // Food containment relationships are in effect as directed acyclic
  // graph (DAG), which is not necessarily connected. We have inside the
  // food_objects container a collection of nodes in this DAG. The goal is
  // to fill out the collection with all of the nodes in the DAG that are
  // reachable from the initial set, and also to devise a partial ordering on
  // this DAG so that the final collection always lists children before their
  // parents. This allows the client to construct the returned objects in
  // the order that they appear in the serialized message without having to
  // worry about dependency issues.

  // This is accomplished by performing simultaneous breadth-first searches
  // starting from each of the initial nodes, and pushing the visited nodes
  // onto a stack in the order visited. Then, the stack is popped so that the
  // final collection contains the visited nodes in the reverse order that they
  // were visited.

  if (!(omissions.single_foods && omissions.composite_foods && omissions.templates))
  {
    QQueue<QSharedPointer<const Food> > bfsQueue;
    QStack<QSharedPointer<const Food> > visitedFoods;
    QSet<QString> enqueuedFoodIds = food_objects.getFoodIds();

    bfsQueue.append(food_objects.getFoods());

    while (!bfsQueue.isEmpty()) {

      QSharedPointer<const Food> food = bfsQueue.dequeue();

      if (food) {

        visitedFoods.push(food);

        QVector<FoodAmount> component_amounts = food->getComponentAmounts();

        for (QVector<FoodAmount>::const_iterator i = component_amounts.begin();
            i != component_amounts.end(); ++i)
        {
          QSharedPointer<const Food> component_food = i->getFood();

          // Optimization: If the BFS queue contains, or previously contained,
          // this food, do not add it to the queue again (this might happen if
          // two foods in the initial set contain the same component food, or if
          // one food in the initial set contains another food in the initial
          // set as a component)

          if (component_food && !enqueuedFoodIds.contains(component_food->getId())) {
            enqueuedFoodIds.insert(food->getId());
            bfsQueue.enqueue(component_food);
          }
        }
      }
    }

    FoodLoadResponseObjects new_food_objects;

    while (!visitedFoods.isEmpty())
    {
      new_food_objects.addFood(visitedFoods.pop());
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
