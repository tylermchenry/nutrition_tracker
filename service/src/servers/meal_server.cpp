#include "servers/meal_server.h"
#include "servers/data_server.h"
#include "libnutrition/data/meal.h"
#include <cassert>

MealLoadResponseObjects::MealLoadResponseObjects()
  : isError(false)
{
}

void MealLoadResponseObjects::addMeal
  (const QSharedPointer<const Meal>& meal)
{
  if (meal && !mealIds.contains(meal->getMealIdTuple())) {
    mealIds.insert(meal->getMealIdTuple());
    meals.append(meal);
  }
}

void MealLoadResponseObjects::addMeals
  (const QVector<QSharedPointer<Meal> >& meals)
{
  for (QVector<QSharedPointer<Meal> >::const_iterator i = meals.begin();
       i != meals.end(); ++i)
  {
    addMeal(*i);
  }
}

void MealLoadResponseObjects::addMeals
  (const QList<QSharedPointer<const Meal> >& meals)
{
  for (QList<QSharedPointer<const Meal> >::const_iterator i = meals.begin();
       i != meals.end(); ++i)
  {
    addMeal(*i);
  }
}

void MealLoadResponseObjects::setError(const QString& errorMessage)
{
  if (isError) {
    this->errorMessage = "Multiple errors occurred while processing this request.";
  } else {
    this->errorMessage = errorMessage;
  }
  isError = true;
}

QList<QSharedPointer<const Meal> > MealLoadResponseObjects::getMeals() const
{
  return meals;
}

MealLoadResponse MealLoadResponseObjects::serialize() const
{
  MealLoadResponse resp;

  for (QList<QSharedPointer<const Meal> >::const_iterator i = meals.begin();
       i != meals.end(); ++i)
  {
    *(resp.add_meals()) = (*i)->serialize();
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
          resp_objs.addMeal(Meal::getMeal
            (userId,
             QDate::fromString(QString::fromStdString(id.date_iso8601()), Qt::ISODate),
             id.mealid()));
        } else {
          resp_objs.addMeals(Meal::getMealsForDay
            (userId,
             QDate::fromString(QString::fromStdString(id.date_iso8601()), Qt::ISODate)));
        }
      }
    }

    return resp_objs;
  }
}
