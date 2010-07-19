/*
 * service_back_end_template.cpp
 * Part of libnutrition
 *
 *  Created on: Jul 16, 2010
 *      Author: Tyler McHenry <tyler@nerdland.net>
 */

#include "service_back_end.h"
#include "libnutrition/proto/service/data_messages.pb.h"
#include "libnutrition/data/impl/template_impl.h"
#include "libnutrition/data/data_cache.h"
#include <stdexcept>

QSharedPointer<Template> ServiceBackEnd::loadTemplate(int id)
{
  DataLoadRequest req;
  DataLoadResponse resp;
  LoadedData ldata;

  DataCache<Template>::getInstance().remove(id);

  req.mutable_templateloadrequest()->add_requestedids(id);

  setOmissions(req);

  writeMessageAndReadResponse(req, resp);
  loadResponseData(ldata, resp);

  return ldata.templates[id];
}

QMultiMap<QString, int> ServiceBackEnd::loadTemplateNamesForUser
  (int userId)
{
  TemplateLoadRequest req;
  TemplateLoadResponse resp;

  req.add_requestedids(userId);
  req.set_nameandidonly(true);

  writeMessageAndReadResponse(req, resp);

  QMultiMap<QString, int> foodNames;

  for (int i = 0; i < resp.templates_size(); ++i) {
    foodNames.insert(QString::fromStdString(resp.templates(i).name()),
                     resp.templates(i).id());
  }

  return foodNames;
}

void ServiceBackEnd::storeTemplate(const QSharedPointer<Template>& templ)
{
  TemplateStoreRequest req;
  TemplateStoreResponse resp;

  *(req.add_templates()) = templ->serialize();

  writeMessageAndReadResponse(req, resp);

  if (resp.has_error() && resp.error().iserror()) {
    throw std::runtime_error("Template store error: " + resp.error().errormessage());
  }

  for (int i = 0; i < resp.storedids_size(); ++i) {
    if (resp.storedids(i) == templ->getTemplateId()) return;
  }

  throw std::runtime_error("Failed to store Template");
}

void ServiceBackEnd::deleteTemplate(const QSharedPointer<Template>& templ)
{
  deleteTemplate(templ->getTemplateId());
}

void ServiceBackEnd::deleteTemplate(int id)
{
  TemplateDeleteRequest req;
  TemplateDeleteResponse resp;

  req.add_deleteids(id);

  writeMessageAndReadResponse(req, resp);

  if (resp.has_error() && resp.error().iserror()) {
    throw std::runtime_error("Template delete error: " + resp.error().errormessage());
  }

  for (int i = 0; i < resp.deletedids_size(); ++i) {
    if (resp.deletedids(i) == id) return;
  }

  throw std::runtime_error("Failed to delete Template");
}

void ServiceBackEnd::loadResponseData
  (LoadedData& loadedData, const TemplateLoadResponse& resp)
{
  if (resp.has_error() && resp.error().iserror()) {
    throw std::runtime_error("Template load error: " + resp.error().errormessage());
  }

  for (int i = 0; i < resp.templates_size(); ++i) {

    const TemplateData& tdata = resp.templates(i);
    int id = tdata.id();

    QSharedPointer<Template> templ =
      DataCache<Template>::getInstance().get(id);

    if (!templ) {

      // For simplicity, there is a source invariant that the values of the
      // enumeration elements in the protocol buffers match the values of the
      // elements in the corresponding enumeration inside a data class.

      QSharedPointer<TemplateImpl> templ_impl;

      templ_impl = QSharedPointer<TemplateImpl>
        (new TemplateImpl
          (id,
           QString::fromStdString(tdata.name()),
           tdata.ownerid()));

      setComponents(templ_impl, tdata.components());

      templ = templ_impl;
      DataCache<Template>::getInstance().insert(id, templ);
    }

    loadedData.templates.insert(id, templ);
  }
}
