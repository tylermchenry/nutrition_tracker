#include "servers/meal_server.h"
#include "servers/data_server.h"
#include "libnutrition/data/meal.h"
#include <cassert>

MealListing::MealListing()
  : isError(false)
{
}

void MealListing::addMeal(const QSharedPointer<const Meal>& meal)
{
  if (meal) addMeal(meal->getMealId(), meal->getName());
}

void MealListing::addMeal(int id, const QString& name)
{
  mealNames[id] = name;
}

void MealListing::addMeals(const QMap<int, QString>& mealNames)
{
  this->mealNames.unite(mealNames); // Note: Might duplicate keys!
}

void MealListing::setError(const QString& errorMessage)
{
  if (isError) {
    this->errorMessage = "Multiple errors occurred while processing this request.";
  } else {
    this->errorMessage = errorMessage;
  }
  isError = true;
}

MealLoadResponse MealListing::serialize() const
{
  MealLoadResponse resp;

  QList<int> uniqueIds = mealNames.uniqueKeys();

  for (QList<int>::const_iterator i = uniqueIds.begin(); i != uniqueIds.end(); ++i)
  {
    MealData* mdata = resp.add_meals();
    mdata->set_mealid(*i);
    mdata->set_name(mealNames[*i].toStdString());
  }

  if (isError) {
    resp.mutable_error()->set_iserror(true);
    resp.mutable_error()->set_errormessage(errorMessage.toStdString());
  }

  return resp;
}

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
    MealListing listing;

    if (req.omit()) return listing;

    assert(req.nameandidonly());

    if (req.all()) {

      listing.addMeals(Meal::getAllMealNames(loggedInUserId, !req.omitgenerics()));

    } else {

      listing.setError("Retrieving names for specific meals is not currently "
                       "supported.");
    }

    return listing;
  }

}
