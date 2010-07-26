#include "servers/composite_food_server.h"

namespace CompositeFoodServer {

  FoodLoadResponseObjects loadCompositeFoods(const CompositeFoodLoadRequest& req)
  {
    FoodLoadResponseObjects resp_objs;

    return loadCompositeFoods(resp_objs, req);
  }

  FoodLoadResponseObjects& loadCompositeFoods
    (FoodLoadResponseObjects& resp_objs, const CompositeFoodLoadRequest& req)
  {
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
      resp_objs.addObject
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

  CompositeFoodListing loadCompositeFoodNames
    (const CompositeFoodLoadRequest& req, int loggedInUserId)
  {
    CompositeFoodListing listing(req.includedatesinname());

    if (req.omit()) return listing;

    assert(req.nameandidonly());

    if (req.omitexpired()) {
      listing.setError("Omission of expired composite foods is not yet supported.");
    }

    for (int i = 0; i < req.requestedids_size(); ++i)
    {
      listing.addObject(CompositeFood::getCompositeFood(req.requestedids(i)));
    }

    bool accessViolation = false;

    for (int i = 0; i < req.requesteduserids_size(); ++i)
    {
      if (req.requesteduserids(i) == loggedInUserId) {
        listing.addObjects(CompositeFood::getFoodsForUser(req.requesteduserids(i)));
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
