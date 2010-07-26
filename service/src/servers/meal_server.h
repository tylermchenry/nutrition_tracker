#ifndef MEAL_SERVER_H_
#define MEAL_SERVER_H_

#include "libnutrition/proto/service/meal_messages.pb.h"
#include "libnutrition/data/meal.h"
#include "servers/response_objects.h"
#include "servers/listing.h"
#include <QString>
#include <QSet>

class MealLoadResponseObjects : public ResponseObjects<Meal, MealLoadResponse>
{
  protected:

    virtual bool isValid
      (const QSharedPointer<const Meal>& meal) const
        { return !meal.isNull(); }

    virtual Meal::MealIdTuple getId
      (const QSharedPointer<const Meal>& meal) const
        { return meal->getMealIdTuple(); }

    virtual void addObjectToResponse
      (MealLoadResponse& resp, const QSharedPointer<const Meal>& meal) const
        { *(resp.add_meals()) = meal->serialize(); }
};

class MealListing : public Listing<Meal, MealLoadResponse, int>
{
  protected:

     virtual int getId
       (const QSharedPointer<const Meal>& meal) const
         { return meal->getMealId(); }

     virtual QString getName
       (const QSharedPointer<const Meal>& meal) const
         { return meal->getName(); }

     virtual void addListingToResponse
       (MealLoadResponse& resp, const int& id, const QString& name) const
         {
           MealData* mdata = resp.add_meals();
           mdata->set_mealid(id);
           mdata->set_name(name.toStdString());
         }
};

namespace MealServer
{
  MealLoadResponseObjects loadMeals
    (const MealLoadRequest& req, int loggedInUserId);

  MealListing loadMealNames
    (const MealLoadRequest& req, int loggedInUserId);
}

#endif /* MEAL_SERVER_H_ */
