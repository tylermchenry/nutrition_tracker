#ifndef DATA_SERVER_H_
#define DATA_SERVER_H_

#include "servers/omissions.h"
#include "servers/food_server.h"
#include "servers/single_food_server.h"
#include "servers/composite_food_server.h"
#include "servers/template_server.h"
#include "servers/meal_server.h"
#include "servers/nutrient_server.h"
#include "servers/unit_server.h"
#include "servers/specialized_unit_server.h"
#include "servers/group_server.h"
#include "servers/user_server.h"
#include "libnutrition/proto/service/data_messages.pb.h"

class DataLoadResponseObjects
{
  public:

    explicit DataLoadResponseObjects(const Omissions& omissions)
      : omissions(omissions)
    {}

    UserLoadResponseObjects user_objects;
    GroupLoadResponseObjects group_objects;
    UnitLoadResponseObjects unit_objects;
    SpecializedUnitLoadResponseObjects specialized_unit_objects;
    NutrientLoadResponseObjects nutrient_objects;
    FoodLoadResponseObjects food_objects;
    MealLoadResponseObjects meal_objects;

    SingleFoodListing single_food_listing;
    CompositeFoodListing composite_food_listing;
    TemplateListing template_listing;
    MealListing meal_listing;

    DataLoadResponse serialize();

  private:

    Omissions omissions;

    void removeDuplicateListings();

    void acquireDependentObjects();

    static QList<QSharedPointer<const Food> > findSubgraphRoots
      (const FoodLoadResponseObjects& food_objects);

    static QList<QSharedPointer<const Food> > reverseTopologicalSort
      (const QList<QSharedPointer<const Food> >& rootFoods);
};

namespace DataServer {

  DataLoadResponseObjects loadData(const DataLoadRequest& req, int loggedInUserId);

}

#endif /* DATA_SERVER_H_ */
