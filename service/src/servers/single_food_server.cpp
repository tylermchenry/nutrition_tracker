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

  void updateSingleFoodNutrients
    (const QSharedPointer<SingleFood>& food, const SingleFoodData& foodData,
     StoredSingleFoodListing& confirmations)
  {
    bool nutrientsUnderspecified = false;
    bool invalidNutrients = false;
    bool invalidNutrientUnits = false;

    for (int i = 0; i < foodData.nutrientamounts_size(); ++i) {

      const SingleFoodData_NutrientAmount& nutrientAmountData =
        foodData.nutrientamounts(i);

      if (nutrientAmountData.has_nutrientid() &&
          nutrientAmountData.has_amount())
      {
        NutrientAmount nutrientAmount
        (Nutrient::getNutrient
         (QString::fromStdString(nutrientAmountData.nutrientid())),
          nutrientAmountData.amount());

        if (!nutrientAmount.isDefined()) {
          invalidNutrients = true;
          continue;
        }

        if (nutrientAmountData.has_unitabbreviation()) {
          QSharedPointer<const Unit> unit = Unit::getUnit
            (QString::fromStdString(nutrientAmountData.unitabbreviation()));

          if (!unit) {
            invalidNutrientUnits = true;
            continue;
          }

          nutrientAmount.setAmount(nutrientAmountData.amount(), unit);
        }

        food->setNutrient(nutrientAmount);
      }
      else
      {
        nutrientsUnderspecified = true;
      }
    }

    if (nutrientsUnderspecified) {
      confirmations.setError("Could not update some nutrients on food " +
                             food->getName() + ". Some nutrients were "
                             "missing IDs or amounts.");
    }

    if (invalidNutrients) {
      confirmations.setError("Could not update some nutrients on food " +
                             food->getName() + ". Some nutrient IDs "
                             "were invalid.");
    }

    if (invalidNutrientUnits) {
      confirmations.setError("Could not update some nutrients on food " +
                             food->getName() + ". Some unit abbreviations "
                             "were invalid.");
    }

  }

  StoredSingleFoodListing storeSingleFoods
    (const SingleFoodStoreRequest& req, int loggedInUserId)
  {
    StoredSingleFoodListing confirmations;
    bool accessViolation = false;
    bool idsMissing = false;
    bool idsInvalid = false;

    for (int i = 0; i < req.singlefoods_size(); ++i) {
      const SingleFoodData& foodData = req.singlefoods(i);

      if (foodData.has_id()) {
        QSharedPointer<SingleFood> food = SingleFood::getSingleFood(foodData.id());

        if (food) {

          if (food->getOwnerId() == loggedInUserId) {

            if (foodData.has_name()) {
              food->setName(QString::fromStdString(foodData.name()));
            }

            if (foodData.has_entrysource()) {
              food->setEntrySource
                (static_cast<SingleFood::EntrySources::EntrySource>
                  (foodData.entrysource()));
            }

            if (foodData.has_groupid()) {
              QSharedPointer<const Group> group = Group::getGroup
                (QString::fromStdString(foodData.groupid()));
              if (group) {
                food->setGroup(group);
              } else {
                confirmations.setError("Could not update group on food " +
                                       food->getName() + ". Supplied group " +
                                       "ID is not valid.");
              }
            }

            if (foodData.has_percentrefuse() &&
                foodData.percentrefuse() != food->getPercentRefuse())
            {
              confirmations.setError("Could not update percent refuse on food " +
                                     food->getName() + ". Changing refuse "
                                     "percentages is not yet supported.");
            }

            if (foodData.has_refusedescription() &&
                QString::fromStdString(foodData.refusedescription()) !=
                  food->getRefuseDescription())
            {
              confirmations.setError("Could not update refuse description on food " +
                                     food->getName() + ". Changing refuse  "
                                     "descriptions is not yet supported.");
            }

            updateSingleFoodNutrients(food, foodData, confirmations);

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

            if ((foodData.has_energydensityfat() &&
                 foodData.energydensityfat() !=
                   food->getCalorieDensityForNutrientId(Nutrient::FAT_NAME)) ||
                (foodData.has_energydensityprotein() &&
                 foodData.energydensityprotein() !=
                   food->getCalorieDensityForNutrientId(Nutrient::PROTEIN_NAME)) ||
                (foodData.has_energydensitycarbohydrate() &&
                 foodData.energydensitycarbohydrate() !=
                   food->getCalorieDensityForNutrientId(Nutrient::CARBOHYDRATE_NAME)) ||
                (foodData.has_energydensityfat() &&
                 foodData.energydensityfat() !=
                   food->getCalorieDensityForNutrientId(Nutrient::ALCOHOL_NAME)))
            {
              confirmations.setError("Could not update energy densities on food " +
                                     food->getName() + ". Changing energy "
                                     "densities is not yet supported.");
            }

            try {
              food->saveToDatabase();
              confirmations.addObject(food);
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

  DeletedSingleFoodListing deleteSingleFoods
    (const SingleFoodDeleteRequest& req, int loggedInUserId)
  {
    DeletedSingleFoodListing confirmations;
    bool accessViolation = false;
    bool idsInvalid = false;

    for (int i = 0; i < req.deleteids_size(); ++i) {

      QSharedPointer<SingleFood> food = SingleFood::getSingleFood(req.deleteids(i));

      if (food) {

        if (food->getOwnerId() == loggedInUserId) {

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
