#include "servers/single_food_server.h"

namespace SingleFoodServer {

  FoodLoadResponseObjects loadSingleFoods(const SingleFoodLoadRequest& req)
  {
    FoodLoadResponseObjects resp_objs;

    return loadSingleFoods(resp_objs, req);
  }

  FoodLoadResponseObjects& loadSingleFoods
    (FoodLoadResponseObjects& resp_objs, const SingleFoodLoadRequest& req)
  {
    if (req.omit()) return resp_objs;

    for (int i = 0; i < req.requestedids_size(); ++i) {
      resp_objs.addObject
        (SingleFood::getSingleFood(req.requestedids(i)));
    }

    if (req.requesteduserids_size() > 0) {
      resp_objs.setError(BackEnd::FoodTypes::SingleFood,
                         "Requesting all single foods for particular user(s) "
                         "without the nameAndIdOnly option is not currently "
                         "supported");
    }

    return resp_objs;
  }

  SingleFoodListing loadSingleFoodNames
    (const SingleFoodLoadRequest& req, int loggedInUserId)
  {
    SingleFoodListing listing;

    if (req.omit()) return listing;

    assert(req.nameandidonly());

    for (int i = 0; i < req.requestedids_size(); ++i)
    {
      listing.addObject(SingleFood::getSingleFood(req.requestedids(i)));
    }

    bool accessViolation = false;

    for (int i = 0; i < req.requesteduserids_size(); ++i)
    {
      if (req.requesteduserids(i) == loggedInUserId) {
        listing.addObjects(SingleFood::getFoodsForUser(req.requesteduserids(i)));
      } else {
        accessViolation = true;
      }
    }

    if (accessViolation) {
      listing.setError("Some requested food names were omitted because they "
                       "belong to another user.");
    }

    return listing;
  }
}
