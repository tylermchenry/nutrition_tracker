#ifndef TEMPLATE_SERVER_H_
#define TEMPLATE_SERVER_H_

#include "libnutrition/proto/service/template_messages.pb.h"
#include "libnutrition/data/template.h"
#include "servers/food_server.h"
#include "servers/listing.h"
#include "servers/modification_listing.h"
#include "servers/update_components.h"
#include <QString>
#include <QSet>

template <typename R>
class TemplateListingBase : public Listing<Template, R>
{
  protected:

    virtual bool isValid
      (const QSharedPointer<const Template>& templ) const
        { return !templ.isNull(); }

    virtual int getId
      (const QSharedPointer<const Template>& templ) const
        { return templ->getTemplateId(); }

    virtual QString getName
      (const QSharedPointer<const Template>& templ) const
        { return templ->getName(); }
};

class TemplateListing
  : public TemplateListingBase<TemplateLoadResponse>
{
  protected:

    virtual void addListingToResponse
      (TemplateLoadResponse& resp, const int& id, const QString& name) const
    {
      TemplateData* tdata = resp.add_templates();
      tdata->set_id(id);
      tdata->set_name(name.toStdString());
    }
};

class StoredTemplateListing
  : public TemplateListingBase<TemplateStoreResponse>,
    public ModificationListing<Template, TemplateStoreResponse>,
    public ComponentModificationListing<int, TemplateStoreResponse>
{
  public:

    virtual TemplateStoreResponse serialize() const
    {
      TemplateStoreResponse resp;
      return serialize(resp);
    }

    virtual TemplateStoreResponse serialize
      (TemplateStoreResponse& resp) const
    {
      TemplateListingBase<TemplateStoreResponse>::serialize(resp);
      ModificationListing<Template, TemplateStoreResponse>::serialize(resp);
      ComponentModificationListing<int, TemplateStoreResponse>::serialize(resp);
      return resp;
    }

  protected:

    virtual void setId(int& collectionId, const int& id) const
      { collectionId = id; }

    virtual void addListingToResponse
      (TemplateStoreResponse& resp, const int& id, const QString&) const
        { resp.add_storedids(id); }
};

class DeletedTemplateListing
  : public TemplateListingBase<TemplateDeleteResponse>
{
  protected:

    virtual void addListingToResponse
      (TemplateDeleteResponse& resp, const int& id, const QString&) const
        { resp.add_deletedids(id); }
};

namespace TemplateServer
{
  FoodLoadResponseObjects loadTemplates(const TemplateLoadRequest& req);

  FoodLoadResponseObjects& loadTemplates
    (FoodLoadResponseObjects& resp_objs, const TemplateLoadRequest& req);

  TemplateListing loadTemplateNames(const TemplateLoadRequest& req);

  StoredTemplateListing storeTemplates(const TemplateStoreRequest& req);

  DeletedTemplateListing deleteTemplates(const TemplateDeleteRequest& req);
}

#endif /* TEMPLATE_SERVER_H_ */
