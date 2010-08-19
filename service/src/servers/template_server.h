#ifndef TEMPLATE_SERVER_H_
#define TEMPLATE_SERVER_H_

#include "libnutrition/proto/service/template_messages.pb.h"
#include "libnutrition/data/template.h"
#include "servers/food_server.h"
#include "servers/listing.h"
#include <QString>
#include <QSet>

class TemplateListing : public Listing<Template, TemplateLoadResponse>
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

    virtual void addListingToResponse
      (TemplateLoadResponse& resp, const int& id, const QString& name) const
    {
      TemplateData* tdata = resp.add_templates();
      tdata->set_id(id);
      tdata->set_name(name.toStdString());
    }
};

namespace TemplateServer
{
  FoodLoadResponseObjects loadTemplates(const TemplateLoadRequest& req);

  FoodLoadResponseObjects& loadTemplates
    (FoodLoadResponseObjects& resp_objs, const TemplateLoadRequest& req);

  TemplateListing loadTemplateNames
    (const TemplateLoadRequest& req);
}

#endif /* TEMPLATE_SERVER_H_ */
