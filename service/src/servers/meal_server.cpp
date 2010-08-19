#include "servers/meal_server.h"
#include "servers/data_server.h"
#include "libnutrition/data/meal.h"
#include <cassert>

namespace MealServer {

  MealLoadResponseObjects loadMeals(const MealLoadRequest& req)
  {
    MealLoadResponseObjects resp_objs;

    if (req.omit()) return resp_objs;

    assert(!req.nameandidonly());

    if (req.all()) {

      resp_objs.setError("Requesting all meals without the nameAndIdOnly option "
                         "is not currently supported");

    } else {

      for (int i = 0; i < req.requestedids_size(); ++i) {

        const MealIdentifier& id = req.requestedids(i);
        int userId;

        if (id.has_userid()) {
          userId = id.userid();
        } else {
          userId = User::getLoggedInUserId();
        }

        if (userId != User::getLoggedInUserId()) {
          resp_objs.setError("Some requested meals were omitted because they "
                             "are owned by another user.");
          continue;
        }

        if (!id.has_date_iso8601()) {
          resp_objs.setError("Some requests were not processed because a date "
                             "was not specified.");
          continue;
        }

        if (id.has_mealid()) {
          resp_objs.addObject(Meal::getMeal
            (userId,
             QDate::fromString(QString::fromStdString(id.date_iso8601()), Qt::ISODate),
             id.mealid()));
        } else {
          resp_objs.addObjects(Meal::getMealsForDay
            (userId,
             QDate::fromString(QString::fromStdString(id.date_iso8601()), Qt::ISODate)));
        }
      }
    }

    return resp_objs;
  }

  MealListing loadMealNames
    (const MealLoadRequest& req)
  {
    MealListing listing(false);

    if (req.omit()) return listing;

    assert(req.nameandidonly());

    if (req.all()) {

      QMap<int, QString> idMap;
      QMap<Meal::MealIdTuple, QString> tupleMap;

      idMap = Meal::getAllMealNames(User::getLoggedInUserId(), !req.omitgenerics());

      for (QMap<int, QString>::const_iterator i = idMap.begin();
           i != idMap.end(); ++i)
      {
        tupleMap[Meal::MealIdTuple(-1, QDate(), i.key())] = i.value();
      }

      listing.addObjects(tupleMap);

    } else {

      listing.setError("Retrieving names for specific meals is not currently "
                       "supported.");
    }

    return listing;
  }

  StoredMealListing storeMeals
    (const MealStoreRequest& req)
  {
    StoredMealListing confirmations;
    bool accessViolation = false;
    bool idsMissing = false;
    bool idsInvalid = false;

    for (int i = 0; i < req.meals_size(); ++i) {
      const MealData& mealData = req.meals(i);

      if (mealData.has_userid() && mealData.has_date_iso8601() && mealData.has_mealid()) {

        QDate date = QDate::fromString
          (QString::fromStdString(mealData.date_iso8601()), Qt::ISODate);

        QSharedPointer<Meal> meal = Meal::getOrCreateMeal
          (mealData.userid(), date, mealData.mealid());

        if (meal) {

          if (meal->getOwnerId() == User::getLoggedInUserId()) {

            // Name is ignored for now
            // TODO: Some way to create/modify meal names

            confirmations.addModifications
              (meal->getMealIdTuple(),
               UpdateComponents::updateComponents(meal, mealData.components()));

            try {
              meal->saveToDatabase();
              confirmations.addObject(meal);
            } catch (const std::exception& ex) {
              confirmations.setError("Failed to store meal " + meal->getName() +
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
      confirmations.setError("Some meal data was not stored because it is not "
                             "owned by the currently logged in user.");
    }

    if (idsMissing) {
      confirmations.setError("Some meal data was not stored because the request "
                             "did not contain a complete meal identifier");
    }

    if (idsInvalid) {
      confirmations.setError("Some meal data was not stored because the "
                             "supplied ID numbers were invalid");
    }

    return confirmations;
  }

  DeletedMealListing deleteMeals
    (const MealDeleteRequest& req)
  {
    DeletedMealListing confirmations;
    bool accessViolation = false;
    bool idsMissing = false;
    bool idsInvalid = false;

    for (int i = 0; i < req.deleteids_size(); ++i) {

      const MealIdentifier& mealId = req.deleteids(i);

      if (mealId.has_userid() && mealId.has_date_iso8601() && mealId.has_mealid()) {

        QDate date = QDate::fromString
          (QString::fromStdString(mealId.date_iso8601()), Qt::ISODate);

        QSharedPointer<Meal> meal = Meal::getMeal
          (mealId.userid(), date, mealId.mealid());

        if (meal) {

          if (meal->getOwnerId() == User::getLoggedInUserId()) {

            try {
              meal->deleteFromDatabase();
              confirmations.addObject(meal);
            } catch (const std::exception& ex) {
              confirmations.setError("Failed to delete meal " + meal->getName() +
                                     " from database. Error was: " +
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
      confirmations.setError("Some meal data was not deleted because it is not "
                             "owned by the currently logged in user.");
    }

    if (idsMissing) {
      confirmations.setError("Some meal data was not stored because the request "
                             "did not contain a complete meal identifier");
    }

    if (idsInvalid) {
      confirmations.setError("Some meal data was not deleted because the "
                             "supplied ID numbers were invalid");
    }

    return confirmations;
  }


}
