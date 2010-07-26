#ifndef SINGLE_FOOD_SERVER_H_
#define SINGLE_FOOD_SERVER_H_

#include "libnutrition/proto/service/single_food_messages.pb.h"
#include "libnutrition/data/single_food.h"
#include "servers/food_server.h"
#include "servers/listing.h"
#include <QString>
#include <QSet>

class SingleFoodListing : public Listing<SingleFood, SingleFoodLoadResponse>
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

    virtual void addListingToResponse
      (SingleFoodLoadResponse& resp, const int& id, const QString& name) const
    {
      SingleFoodData* sfdata = resp.add_singlefoods();
      sfdata->set_id(id);
      sfdata->set_name(name.toStdString());
    }
};

namespace SingleFoodServer
{
  FoodLoadResponseObjects loadSingleFoods(const SingleFoodLoadRequest& req);

  FoodLoadResponseObjects& loadSingleFoods
    (FoodLoadResponseObjects& resp_objs, const SingleFoodLoadRequest& req);

  SingleFoodListing loadSingleFoodNames
    (const SingleFoodLoadRequest& req, int loggedInUserId);
}

#endif /* SINGLE_FOOD_SERVER_H_ */
