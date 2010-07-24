#include "servers/data_server.h"
#include "servers/omissions.h"
#include "libnutrition/data/food_collection.h"
#include <QStack>
#include <QQueue>

DataLoadResponse DataLoadResponseObjects::serialize()
{
  DataLoadResponse resp;

  acquireDependentObjects();

  if (!omissions.users && !user_objects.isEmpty()) {
    *(resp.mutable_userloadresponse()) = user_objects.serialize();
  }

  if (!omissions.groups && !group_objects.isEmpty()) {
    *(resp.mutable_grouploadresponse()) = group_objects.serialize();
  }

  if (!omissions.units && !unit_objects.isEmpty()) {
    *(resp.mutable_unitloadresponse()) = unit_objects.serialize();
  }

  if (!omissions.specializedUnits && !specialized_unit_objects.isEmpty()) {
    *(resp.mutable_specializedunitloadresponse()) =
        specialized_unit_objects.serialize();
  }

  if (!omissions.nutrients && !nutrient_objects.isEmpty()) {
    *(resp.mutable_nutrientloadresponse()) = nutrient_objects.serialize();
  }

  if (!omissions.meals && !meal_objects.isEmpty()) {
    *(resp.mutable_mealloadresponse()) = meal_objects.serialize();
  }

  if (!food_objects.isEmpty())
  {
    food_objects.serialize(resp, omissions);
  }

  return resp;
}

void DataLoadResponseObjects::acquireDependentObjects()
{
  bool omitAllFoods =
    (omissions.single_foods && omissions.composite_foods && omissions.templates);

  // Meals acquire Foods (singles, composites, templates)

  if (!omissions.meals && !omitAllFoods) {

    QList<QSharedPointer<const Meal> > meals = meal_objects.getObjects();

    for (QList<QSharedPointer<const Meal> >::const_iterator i = meals.begin();
         i != meals.end(); ++i)
    {
      QList<FoodComponent> components = (*i)->getComponents();

      for (QList<FoodComponent>::const_iterator j = components.begin();
           j != components.end(); ++j)
      {
        food_objects.addObject(j->getFoodAmount().getFood());
      }
    }
  }

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

  // This is accomplished by first discovering which foods from the original set
  // are not dependencies of any other foods in the original set. Then, the
  // subgraph of the original foods and all of their dependencies is
  // topologically sorted in reverse, using the independent original foods as
  // roots, to obtain an ordering where all dependencies appear before the foods
  // that depend on them. This is the appropriate order for serialization.

  if (!omitAllFoods) {
    food_objects.replaceObjects
      (reverseTopologicalSort(findSubgraphRoots(food_objects)));
  }

  // Foods acquire Nutrients

  if (!omissions.nutrients && !omitAllFoods) {

    QList<QSharedPointer<const Food> > foods = food_objects.getObjects();

    for (QList<QSharedPointer<const Food> >::const_iterator i = foods.begin();
         i != foods.end(); ++i)
    {
      // Only need to acquire nutrients from foods with no components, since
      // we know that as a result of the aquisition above, all components also
      // appear somewhere in this list.

      if ((*i)->getComponents().isEmpty()) {

        QList<NutrientAmount> nutrientAmounts = (*i)->getNutrients().values();

        for (QList<NutrientAmount>::const_iterator j = nutrientAmounts.begin();
             j != nutrientAmounts.end(); ++j)
        {
          nutrient_objects.addObject(j->getNutrient());
        }
      }
    }
  }

  // Foods acquire Groups

  if (!omissions.groups && !omitAllFoods) {
    // TODO: Figure out how to do this without dynamic_cast since only
    // SingleFood objects have groups.
  }

  // Nutrients and Specialized Units acquire Units

  if (!omissions.units) {
    if (!omissions.nutrients) {

      QList<QSharedPointer<const Nutrient> > nutrients = nutrient_objects.getObjects();

      for (QList<QSharedPointer<const Nutrient> >::const_iterator i = nutrients.begin();
           i != nutrients.end(); ++i)
      {
        unit_objects.addObject((*i)->getStandardUnit());
      }
    }

    if (!omissions.specializedUnits) {
      QList<QSharedPointer<const SpecializedUnit> > specializedUnits =
        specialized_unit_objects.getObjects();

      for (QList<QSharedPointer<const SpecializedUnit> >::const_iterator i =
          specializedUnits.begin(); i != specializedUnits.end(); ++i)
      {
        unit_objects.addObject((*i)->getBaseAmount().getUnit());
      }
    }
  }
}

QList<QSharedPointer<const Food> > DataLoadResponseObjects::findSubgraphRoots
  (const FoodLoadResponseObjects& food_objects)
{
  // This performs a breadth-first search to discover which foods in the set of
  // food_objects are not dependencies, directly or indirectly, of any other
  // foods in the food_objects collection. If food_objects is nonempty, then
  // here must be at least one of these, otherwise the food graph would have a
  // cycle (and then the data store would need to be fixed).

  QList<QSharedPointer<const Food> > originalFoods = food_objects.getObjects();
  QSet<QString> originalFoodIds = food_objects.getObjectIds();

  QQueue<QSharedPointer<const Food> > bfsQueue;
  QSet<QString> originalFoodIdsWithIncomingEdges;
  QSet<QString> enqueuedFoodIds;

  bfsQueue.append(originalFoods);
  enqueuedFoodIds = originalFoodIds;

  while (!bfsQueue.isEmpty()) {

    QSharedPointer<const Food> food = bfsQueue.dequeue();

    if (food) {

      QVector<FoodAmount> component_amounts = food->getComponentAmounts();

      for (QVector<FoodAmount>::const_iterator i = component_amounts.begin();
      i != component_amounts.end(); ++i)
      {
        QSharedPointer<const Food> component_food = i->getFood();

        if (component_food && !enqueuedFoodIds.contains(component_food->getId())) {
          enqueuedFoodIds.insert(food->getId());
          bfsQueue.enqueue(component_food);
        }

        if (component_food && originalFoodIds.contains(component_food->getId())) {
          originalFoodIdsWithIncomingEdges.insert(component_food->getId());
        }
      }
    }
  }

  QList<QSharedPointer<const Food> > rootFoods;

  for (QList<QSharedPointer<const Food> >::const_iterator i = originalFoods.begin();
  i != originalFoods.end(); ++i)
  {
    if (*i && !originalFoodIdsWithIncomingEdges.contains((*i)->getId())) {
      rootFoods.append(*i);
    }
  }

  return rootFoods;
}

QList<QSharedPointer<const Food> > DataLoadResponseObjects::reverseTopologicalSort
  (const QList<QSharedPointer<const Food> >& rootFoods)
{
  // This produces a reverse topological ordering (a postordering) of the DAG
  // consisting of nodes reachable from the given roots, i.e. dependencies will
  // appear before the foods that depend on them
  //
  // Precondition: items in rootFoods are not dependencies of any food that can
  // be reached from any of the other root foods, i.e. root foods have no
  // incoming edges in the subgraph.

  QQueue<QSharedPointer<const Food> > rootFoodsQueue;
  QList<QSharedPointer<const Food> > ordering;
  QSet<QString> pushedFoodIds;

  rootFoodsQueue.append(rootFoods);

  while (!rootFoodsQueue.isEmpty()) {

    QSharedPointer<const Food> rootFood = rootFoodsQueue.dequeue();
    QStack<QSharedPointer<const Food> > dfsStack;

    if (!rootFood) continue;

    dfsStack.push(rootFood);
    pushedFoodIds.insert(rootFood->getId());

    while (!dfsStack.isEmpty()) {

      QSharedPointer<const Food> food = dfsStack.pop();

      if (food) {

        ordering.prepend(food); // Prepending makes the ordering backwards

        QVector<FoodAmount> component_amounts = food->getComponentAmounts();

        for (QVector<FoodAmount>::const_iterator i = component_amounts.begin();
            i != component_amounts.end(); ++i)
        {
          QSharedPointer<const Food> component_food = i->getFood();

          if (component_food && !pushedFoodIds.contains(component_food->getId())) {
            dfsStack.push(component_food);
            pushedFoodIds.insert(food->getId());
          }
        }
      }
    }
  }

  return ordering;
}

namespace DataServer {

  DataLoadResponseObjects loadData(const DataLoadRequest& req, int loggedInUserId)
  {
    DataLoadResponseObjects resp_objs((Omissions(req)));

    if (req.has_userloadrequest()) {
      resp_objs.user_objects = UserServer::loadUsers(req.userloadrequest());
    }

    if (req.has_grouploadrequest()) {
      resp_objs.group_objects = GroupServer::loadGroups(req.grouploadrequest());
    }

    if (req.has_unitloadrequest()) {
      resp_objs.unit_objects = UnitServer::loadUnits(req.unitloadrequest());
    }

    if (req.has_specializedunitloadrequest()) {
      resp_objs.specialized_unit_objects =
        SpecializedUnitServer::loadSpecializedUnits
          (req.specializedunitloadrequest());
    }

    if (req.has_nutrientloadrequest()) {
      resp_objs.nutrient_objects =
        NutrientServer::loadNutrients(req.nutrientloadrequest());
    }

    if (req.has_mealloadrequest()) {
      resp_objs.meal_objects = MealServer::loadMeals
        (req.mealloadrequest(), loggedInUserId);
    }

    if (req.has_singlefoodloadrequest()) {
      SingleFoodServer::loadSingleFoods
        (resp_objs.food_objects, req.singlefoodloadrequest());
    }

    if (req.has_compositefoodloadrequest()) {
      CompositeFoodServer::loadCompositeFoods
        (resp_objs.food_objects, req.compositefoodloadrequest());
    }

    if (req.has_templateloadrequest()) {
      TemplateServer::loadTemplates
        (resp_objs.food_objects, req.templateloadrequest());
    }

    return resp_objs;
  }

}
