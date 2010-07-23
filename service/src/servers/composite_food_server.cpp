#include "servers/composite_food_server.h"

namespace CompositeFoodServer {

  FoodLoadResponseObjects loadCompositeFoods(const CompositeFoodLoadRequest& req)
  {
    FoodLoadResponseObjects resp_objs;

    if (req.omit()) return resp_objs;

    if (req.omitexpired()) {
      resp_objs.setError(BackEnd::FoodTypes::CompositeFood,
                         "The omitExpired option is ignored when the "
                         "nameAndIdOnly option is not set.");
    }

    if (req.includedatesinname()) {
      resp_objs.setError(BackEnd::FoodTypes::CompositeFood,
                         "The includeDatesInName option is ignored when the "
                         "nameAndIdOnly option is not set.");
    }

    for (int i = 0; i < req.requestedids_size(); ++i) {
      resp_objs.addFood
        (CompositeFood::getCompositeFood(req.requestedids(i)));
    }

    if (req.requestedids_size() > 0) {
      resp_objs.setError(BackEnd::FoodTypes::CompositeFood,
                         "Requesting all composite foods for particular user(s) "
                         "without the nameAndIdOnly option is not currently "
                         "supported");
    }

    return resp_objs;
  }
}
