#ifndef COMPOSITE_FOOD_SERVER_H_
#define COMPOSITE_FOOD_SERVER_H_

#include "libnutrition/proto/service/composite_food_messages.pb.h"
#include "libnutrition/data/composite_food.h"
#include "servers/food_server.h"
#include "servers/listing.h"
#include <QString>
#include <QSet>

class CompositeFoodListing : public Listing<CompositeFood, CompositeFoodLoadResponse>
{
  public:

    explicit CompositeFoodListing(bool includeDatesInName = false)
      : includeDatesInName(includeDatesInName)
    {}

    void setIncludeDatesInName(bool includeDatesInName)
    { this->includeDatesInName = includeDatesInName; }

  protected:

    virtual bool isValid
      (const QSharedPointer<const CompositeFood>& compositeFood) const
        { return !compositeFood.isNull(); }

    virtual int getId
      (const QSharedPointer<const CompositeFood>& compositeFood) const
        { return compositeFood->getCompositeFoodId(); }

    virtual QString getName
      (const QSharedPointer<const CompositeFood>& compositeFood) const
    {
      if (!includeDatesInName) {
        return compositeFood->getName();
      } else {
        return compositeFood->getName() +
          CompositeFood::generateExpirySuffix
            (compositeFood->getCreationDate(), compositeFood->getExpiryDate());
      }
    }

    virtual void addListingToResponse
      (CompositeFoodLoadResponse& resp, const int& id, const QString& name) const
    {
      CompositeFoodData* cfdata = resp.add_compositefoods();
      cfdata->set_id(id);
      cfdata->set_name(name.toStdString());
    }

  private:

    bool includeDatesInName;
};


namespace CompositeFoodServer
{
  FoodLoadResponseObjects loadCompositeFoods(const CompositeFoodLoadRequest& req);

  FoodLoadResponseObjects& loadCompositeFoods
    (FoodLoadResponseObjects& resp_objs, const CompositeFoodLoadRequest& req);

  CompositeFoodListing loadCompositeFoodNames
    (const CompositeFoodLoadRequest& req, int loggedInUserId);
}

#endif /* COMPOSITE_FOOD_SERVER_H_ */
