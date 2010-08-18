#ifndef COMPOSITE_FOOD_SERVER_H_
#define COMPOSITE_FOOD_SERVER_H_

#include "libnutrition/proto/service/composite_food_messages.pb.h"
#include "libnutrition/data/composite_food.h"
#include "servers/food_server.h"
#include "servers/listing.h"
#include "servers/update_components.h"
#include <QString>
#include <QSet>

template <typename R>
class CompositeFoodListingBase : public Listing<CompositeFood, R>
{
  protected:

    virtual bool isValid
      (const QSharedPointer<const CompositeFood>& compositeFood) const
        { return !compositeFood.isNull(); }

    virtual int getId
      (const QSharedPointer<const CompositeFood>& compositeFood) const
        { return compositeFood->getCompositeFoodId(); }

    virtual QString getName
      (const QSharedPointer<const CompositeFood>& compositeFood) const
        { return compositeFood->getName(); }
};

class CompositeFoodListing
  : public CompositeFoodListingBase<CompositeFoodLoadResponse>
{
  public:

    explicit CompositeFoodListing(bool includeDatesInName = false)
      : includeDatesInName(includeDatesInName)
    {}

    void setIncludeDatesInName(bool includeDatesInName)
      { this->includeDatesInName = includeDatesInName; }

  protected:

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

class StoredCompositeFoodListing
  : public CompositeFoodListingBase<CompositeFoodStoreResponse>,
    public ComponentModificationListing<int, CompositeFoodStoreResponse>
{
  public:

    virtual CompositeFoodStoreResponse serialize() const
    {
      CompositeFoodStoreResponse resp;
      return serialize(resp);
    }

    virtual CompositeFoodStoreResponse serialize
      (CompositeFoodStoreResponse& resp) const
    {
      CompositeFoodListingBase<CompositeFoodStoreResponse>::serialize(resp);
      ComponentModificationListing<int, CompositeFoodStoreResponse>::serialize
        (resp);
      return resp;
    }

  protected:

    virtual void addListingToResponse
      (CompositeFoodStoreResponse& resp, const int& id, const QString&) const
        { resp.add_storedids(id); }
};

class DeletedCompositeFoodListing
  : public CompositeFoodListingBase<CompositeFoodDeleteResponse>
{
  protected:

    virtual void addListingToResponse
      (CompositeFoodDeleteResponse& resp, const int& id, const QString&) const
        { resp.add_deletedids(id); }
};

namespace CompositeFoodServer
{
  FoodLoadResponseObjects loadCompositeFoods(const CompositeFoodLoadRequest& req);

  FoodLoadResponseObjects& loadCompositeFoods
    (FoodLoadResponseObjects& resp_objs, const CompositeFoodLoadRequest& req);

  CompositeFoodListing loadCompositeFoodNames
    (const CompositeFoodLoadRequest& req, int loggedInUserId);

  StoredCompositeFoodListing storeCompositeFoods
    (const CompositeFoodStoreRequest& req, int loggedInUserId);

  DeletedCompositeFoodListing deleteCompositeFoods
    (const CompositeFoodDeleteRequest& req, int loggedInUserId);
}

#endif /* COMPOSITE_FOOD_SERVER_H_ */
