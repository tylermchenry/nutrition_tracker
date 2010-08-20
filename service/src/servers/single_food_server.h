#ifndef SINGLE_FOOD_SERVER_H_
#define SINGLE_FOOD_SERVER_H_

#include "libnutrition/proto/service/single_food_messages.pb.h"
#include "libnutrition/data/single_food.h"
#include "servers/food_server.h"
#include "servers/listing.h"
#include "servers/modification_listing.h"
#include <QString>
#include <QSet>

template <typename R>
class SingleFoodListingBase : public Listing<SingleFood, R>
{
  protected:

    virtual bool isValid
      (const QSharedPointer<const SingleFood>& singleFood) const
        { return !singleFood.isNull(); }

    virtual int getId
      (const QSharedPointer<const SingleFood>& singleFood) const
        { return singleFood->getSingleFoodId(); }

    virtual QString getName
      (const QSharedPointer<const SingleFood>& singleFood) const
        { return singleFood->getName(); }
};

class SingleFoodListing : public SingleFoodListingBase<SingleFoodLoadResponse>
{
  protected:

    virtual void addListingToResponse
      (SingleFoodLoadResponse& resp, const int& id, const QString& name) const
    {
      SingleFoodData* sfdata = resp.add_singlefoods();
      sfdata->set_id(id);
      sfdata->set_name(name.toStdString());
    }
};

class StoredSingleFoodListing
  : public SingleFoodListingBase<SingleFoodStoreResponse>,
    public ModificationListing<SingleFood, SingleFoodStoreResponse>
{
  public:

    virtual SingleFoodStoreResponse serialize() const
    {
      SingleFoodStoreResponse resp;
      return serialize(resp);
    }

    virtual SingleFoodStoreResponse serialize
      (SingleFoodStoreResponse& resp) const
    {
      SingleFoodListingBase<SingleFoodStoreResponse>::serialize(resp);
      ModificationListing<SingleFood, SingleFoodStoreResponse>::serialize(resp);
      return resp;
    }

  protected:

    virtual void addListingToResponse
      (SingleFoodStoreResponse& resp, const int& id, const QString&) const
        { resp.add_storedids(id); }
};

class DeletedSingleFoodListing : public SingleFoodListingBase<SingleFoodDeleteResponse>
{
  protected:

    virtual void addListingToResponse
      (SingleFoodDeleteResponse& resp, const int& id, const QString&) const
        { resp.add_deletedids(id); }
};

namespace SingleFoodServer
{
  FoodLoadResponseObjects loadSingleFoods(const SingleFoodLoadRequest& req);

  FoodLoadResponseObjects& loadSingleFoods
    (FoodLoadResponseObjects& resp_objs, const SingleFoodLoadRequest& req);

  SingleFoodListing loadSingleFoodNames
    (const SingleFoodLoadRequest& req);

  StoredSingleFoodListing storeSingleFoods
    (const SingleFoodStoreRequest& req);

  DeletedSingleFoodListing deleteSingleFoods
    (const SingleFoodDeleteRequest& req);
}

#endif /* SINGLE_FOOD_SERVER_H_ */
