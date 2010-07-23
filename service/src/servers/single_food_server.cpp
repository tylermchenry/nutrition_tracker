#include "servers/single_food_server.h"

namespace SingleFoodServer {

  FoodLoadResponseObjects loadSingleFoods(const SingleFoodLoadRequest& req)
  {
    FoodLoadResponseObjects resp_objs;

    if (req.omit()) return resp_objs;

    for (int i = 0; i < req.requestedids_size(); ++i) {
      resp_objs.addObject
        (SingleFood::getSingleFood(req.requestedids(i)));
    }

    if (req.requestedids_size() > 0) {
      resp_objs.setError(BackEnd::FoodTypes::SingleFood,
                         "Requesting all single foods for particular user(s) "
                         "without the nameAndIdOnly option is not currently "
                         "supported");
    }

    return resp_objs;
  }
}
