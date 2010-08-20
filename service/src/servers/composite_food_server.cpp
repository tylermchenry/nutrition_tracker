#include "servers/composite_food_server.h"
#include "servers/update_components.h"
#include <libnutrition/data/impl/composite_food_impl.h>

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

    if (req.requesteduserids_size() > 0) {
      resp_objs.setError(BackEnd::FoodTypes::CompositeFood,
                         "Requesting all composite foods for particular user(s) "
                         "without the nameAndIdOnly option is not currently "
                         "supported");
    }

    return resp_objs;
  }

  CompositeFoodListing loadCompositeFoodNames
    (const CompositeFoodLoadRequest& req)
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
      if (req.requesteduserids(i) == User::getLoggedInUserId()) {
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

  StoredCompositeFoodListing storeCompositeFoods
    (const CompositeFoodStoreRequest& req)
  {
    StoredCompositeFoodListing confirmations;
    bool accessViolation = false;
    bool idsMissing = false;
    bool idsInvalid = false;

    for (int i = 0; i < req.compositefoods_size(); ++i) {
      const CompositeFoodData& foodData = req.compositefoods(i);

      if (foodData.has_id()) {
        QSharedPointer<CompositeFood> food = CompositeFood::getCompositeFood(foodData.id());
        UpdateComponents::ComponentModifications cmods;

        if (!food) {
          if (foodData.isnonce()) {
            food = CompositeFood::createNewNonceCompositeFood();
          } else {
            food = CompositeFood::createNewCompositeFood();
          }
        }

        if (food) {

          if (food->getOwnerId() == User::getLoggedInUserId()) {

            if (foodData.has_name()) {
              food->setName(QString::fromStdString(foodData.name()));
            }

            cmods =
              UpdateComponents::updateComponents(food, foodData.components());

            if (foodData.has_weightamount()) {
              food->setBaseAmount
                (foodData.weightamount(),
                 Unit::getPreferredUnit(Unit::Dimensions::Weight));
            }

            if (foodData.has_volumeamount()) {
              food->setBaseAmount
                (foodData.volumeamount(),
                 Unit::getPreferredUnit(Unit::Dimensions::Volume));
            }

            if (foodData.has_quantityamount()) {
              food->setBaseAmount
                (foodData.quantityamount(),
                 Unit::getPreferredUnit(Unit::Dimensions::Quantity));
            }

            if (foodData.has_servingamount()) {
              food->setBaseAmount
                (foodData.servingamount(),
                 Unit::getPreferredUnit(Unit::Dimensions::Serving));
            }

            if (foodData.has_creationdate_iso8601()) {
              food->setCreationDate
                (QDate::fromString
                   (QString::fromStdString(foodData.creationdate_iso8601()),
                    Qt::ISODate));
            }

            if (foodData.has_expirydate_iso8601()) {
              food->setExpiryDate
                (QDate::fromString
                  (QString::fromStdString(foodData.expirydate_iso8601()),
                   Qt::ISODate));
            }

            if (foodData.has_isnonce() && foodData.isnonce() != food->isNonce())
            {
              confirmations.setError("Could not change nonce status of food " +
                                     food->getName() + ". Changing nonce "
                                     "status is not supported.");
            }

            try {
              food->saveToDatabase();
              confirmations.addObject(food);
              confirmations.addModifications
                (food->getCompositeFoodId(),
                 UpdateComponents::updateComponentModifications(food, cmods));
            } catch (const std::exception& ex) {
              confirmations.setError("Failed to store food " + food->getName() +
                                     " to database. Error was: " +
                                     QString::fromStdString(ex.what()));
            }

          } else {
            accessViolation = true;
          }
        } else {
          idsInvalid = true;
        }
      } else {
        idsMissing = true;
      }
    }

    if (accessViolation) {
      confirmations.setError("Some food data was not stored because it is not "
                             "owned by the currently logged in user.");
    }

    if (idsMissing) {
      confirmations.setError("Some food data was not stored because the request "
                             "contained no ID number");
    }

    if (idsInvalid) {
      confirmations.setError("Some food data was not stored because the "
                             "supplied ID numbers were invalid");
    }

    return confirmations;
  }

  DeletedCompositeFoodListing deleteCompositeFoods
    (const CompositeFoodDeleteRequest& req)
  {
    DeletedCompositeFoodListing confirmations;
    bool accessViolation = false;
    bool idsInvalid = false;

    for (int i = 0; i < req.deleteids_size(); ++i) {

      QSharedPointer<CompositeFood> food = CompositeFood::getCompositeFood(req.deleteids(i));

      if (food) {

        if (food->getOwnerId() == User::getLoggedInUserId()) {

          try {
            // TODO: Prevent users from deleting foods that are used by other
            //        users, or silently make system-owned copies
            food->deleteFromDatabase();
            confirmations.addObject(food);
          } catch (const std::exception& ex) {
            confirmations.setError("Failed to delete food " + food->getName() +
                                   " from database. Error was: " +
                                   QString::fromStdString(ex.what()));
          }

        } else {
          accessViolation = true;
        }
      } else {
        idsInvalid = true;
      }
    }

    if (accessViolation) {
      confirmations.setError("Some food data was not deleted because it is not "
                             "owned by the currently logged in user.");
    }

    if (idsInvalid) {
      confirmations.setError("Some food data was not deleted because the "
                             "supplied ID numbers were invalid");
    }

    return confirmations;
  }
}
