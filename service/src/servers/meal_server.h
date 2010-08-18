#ifndef MEAL_SERVER_H_
#define MEAL_SERVER_H_

#include "libnutrition/proto/service/meal_messages.pb.h"
#include "libnutrition/data/meal.h"
#include "servers/response_objects.h"
#include "servers/listing.h"
#include "servers/update_components.h"
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

template <typename R>
class MealListingBase : public Listing<Meal, R>
{
  protected:

    virtual bool isValid
      (const QSharedPointer<const Meal>& meal) const
        { return !meal.isNull(); }

     virtual Meal::MealIdTuple getId
       (const QSharedPointer<const Meal>& meal) const
         { return meal->getMealIdTuple(); }

     virtual QString getName
       (const QSharedPointer<const Meal>& meal) const
         { return meal->getName(); }
};

class MealListing
  : public MealListingBase<MealLoadResponse>
{
  public:

    MealListing(bool includeUserAndDate = true)
      : includeUserAndDate(includeUserAndDate)
    {}

  protected:

    virtual void addListingToResponse
      (MealLoadResponse& resp, const Meal::MealIdTuple& id, const QString& name) const
    {
      MealData* mdata = resp.add_meals();
      mdata->set_mealid(id.mealId);
      if (includeUserAndDate) {
        mdata->set_userid(id.userId);
        mdata->set_date_iso8601(id.date.toString(Qt::ISODate).toStdString());
      }
      mdata->set_name(name.toStdString());
    }

  private:

    bool includeUserAndDate;
};

class StoredMealListing
  : public MealListingBase<MealStoreResponse>,
    public ComponentModificationListing<Meal::MealIdTuple, MealStoreResponse,
            MealIdentifier*>
{
  public:

    virtual MealStoreResponse serialize() const
    {
      MealStoreResponse resp;
      return serialize(resp);
    }

    virtual MealStoreResponse serialize
      (MealStoreResponse& resp) const
    {
      MealListingBase<MealStoreResponse>::serialize(resp);
      ComponentModificationListing<Meal::MealIdTuple, MealStoreResponse,
        MealIdentifier*>::serialize(resp);
      return resp;
    }

  protected:

    virtual void setId
      (MealIdentifier*& collectionId, const Meal::MealIdTuple& id) const
      {
        collectionId->set_mealid(id.mealId);
        collectionId->set_userid(id.userId);
        collectionId->set_date_iso8601(id.date.toString(Qt::ISODate).toStdString());
      }

    virtual void addListingToResponse
      (MealStoreResponse& resp, const Meal::MealIdTuple& id, const QString&) const
        {
          MealIdentifier* mid = resp.add_storedids();
          setId(mid, id);
        }
};

class DeletedMealListing
  : public MealListingBase<MealDeleteResponse>
{
  protected:

    virtual void addListingToResponse
      (MealDeleteResponse& resp, const Meal::MealIdTuple& id, const QString&) const
        {
          MealIdentifier* mid = resp.add_deletedids();
          mid->set_mealid(id.mealId);
          mid->set_userid(id.userId);
          mid->set_date_iso8601(id.date.toString(Qt::ISODate).toStdString());
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
