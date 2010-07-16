/*
 * service_back_end_meal.cpp
 * Part of libnutrition
 *
 *  Created on: Jul 16, 2010
 *      Author: Tyler McHenry <tyler@nerdland.net>
 */

#include "service_back_end.h"
#include "libnutrition/proto/data.pb.h"
#include "libnutrition/data/impl/meal_impl.h"
#include "libnutrition/data/data_cache.h"
#include <stdexcept>

QSharedPointer<Meal> ServiceBackEnd::loadMeal
  (int userId, const QDate& date, int mealId)
{
  DataLoadRequest req;
  DataLoadResponse resp;
  LoadedData ldata;

  Meal::MealIdTuple idTuple(userId, date, mealId);

  DataCache<Meal>::getInstance().remove(idTuple);

  MealIdentifier* id = req.mutable_mealloadrequest()->add_requestedids();

  id->set_userid(userId);
  id->set_date_iso8601(date.toString(Qt::ISODate).toStdString());
  id->set_mealid(mealId);

  setOmissions(req);

  writeMessageAndReadResponse(req, resp);
  loadResponseData(ldata, resp);

  return ldata.meals[idTuple];
}

QMap<int, QString> ServiceBackEnd::loadAllMealNames()
{
  return loadAllMealNamesForUser(-1, true);
}

QMap<int, QString> ServiceBackEnd::loadAllMealNamesForUser
  (int userId, bool includeGenerics)
{
  MealLoadRequest req;
  MealLoadResponse resp;

  req.set_omitgenerics(!includeGenerics);
  req.set_nameandidonly(true);

  if (userId >= 0) {
    MealIdentifier* id = req.add_requestedids();
    id->set_userid(userId);
  } else {
    req.set_all(true);
  }

  writeMessageAndReadResponse(req, resp);

  QMap<int, QString> foodNames;

  for (int i = 0; i < resp.meals_size(); ++i) {
    foodNames.insert(resp.meals(i).mealid(),
                     QString::fromStdString(resp.meals(i).name()));
  }

  return foodNames;
}

QList<QSharedPointer<Meal> > ServiceBackEnd::loadAllMealsForDay
  (int userId, const QDate& date)
{
  DataLoadRequest req;
  DataLoadResponse resp;
  LoadedData ldata;

  // TODO: Remove all meals for this user and day from DataCache

  MealIdentifier* id = req.mutable_mealloadrequest()->add_requestedids();

  id->set_userid(userId);
  id->set_date_iso8601(date.toString(Qt::ISODate).toStdString());

  setOmissions(req);

  writeMessageAndReadResponse(req, resp);
  loadResponseData(ldata, resp);

  return ldata.meals.values();
}

void ServiceBackEnd::loadResponseData
  (LoadedData& loadedData, const MealLoadResponse& resp)
{
  if (resp.has_error() && resp.error().iserror()) {
    throw std::runtime_error("Meal load error: " + resp.error().errormessage());
  }

  for (int i = 0; i < resp.meals_size(); ++i) {

    const MealData& mdata = resp.meals(i);
    Meal::MealIdTuple idTuple(mdata.mealid(),
                              QDate::fromString(QString::fromStdString(mdata.date_iso8601()), Qt::ISODate),
                              mdata.userid());

    QSharedPointer<Meal> meal =
      DataCache<Meal>::getInstance().get(idTuple);

    if (!meal) {

      // For simplicity, there is a source invariant that the values of the
      // enumeration elements in the protocol buffers match the values of the
      // elements in the corresponding enumeration inside a data class.

      QSharedPointer<MealImpl> meal_impl;

      meal_impl = QSharedPointer<MealImpl>
        (new MealImpl
          (idTuple.mealId,
           mdata.creatorid(),
           QString::fromStdString(mdata.name()),
           idTuple.userId,
           idTuple.date));

      setComponents(meal_impl, mdata.components());

      meal = meal_impl;
      DataCache<Meal>::getInstance().insert(idTuple, meal);
    }

    loadedData.meals.insert(idTuple, meal);
  }
}
