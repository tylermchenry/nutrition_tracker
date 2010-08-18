#include "servers/meal_server.h"
#include "servers/data_server.h"
#include "libnutrition/data/meal.h"
#include <cassert>

namespace MealServer {

  MealLoadResponseObjects loadMeals(const MealLoadRequest& req, int loggedInUserId)
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
          userId = loggedInUserId;
        }

        if (userId != loggedInUserId) {
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
    (const MealLoadRequest& req, int loggedInUserId)
  {
    MealListing listing(false);

    if (req.omit()) return listing;

    assert(req.nameandidonly());

    if (req.all()) {

      QMap<int, QString> idMap;
      QMap<Meal::MealIdTuple, QString> tupleMap;

      idMap = Meal::getAllMealNames(loggedInUserId, !req.omitgenerics());

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

}
