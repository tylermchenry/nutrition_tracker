#include "servers/template_server.h"

namespace TemplateServer {

  FoodLoadResponseObjects loadTemplates(const TemplateLoadRequest& req)
  {
    FoodLoadResponseObjects resp_objs;

    return loadTemplates(resp_objs, req);
  }

  FoodLoadResponseObjects& loadTemplates
    (FoodLoadResponseObjects& resp_objs, const TemplateLoadRequest& req)
  {
    if (req.omit()) return resp_objs;

    for (int i = 0; i < req.requestedids_size(); ++i) {
      resp_objs.addObject
        (Template::getTemplate(req.requestedids(i)));
    }

    if (req.requesteduserids_size() > 0) {
      resp_objs.setError(BackEnd::FoodTypes::Template,
                         "Requesting all templates for particular user(s) "
                         "without the nameAndIdOnly option is not currently "
                         "supported");
    }

    return resp_objs;
  }

  TemplateListing loadTemplateNames
    (const TemplateLoadRequest& req)
  {
    TemplateListing listing;

    if (req.omit()) return listing;

    assert(req.nameandidonly());

    for (int i = 0; i < req.requestedids_size(); ++i)
    {
      listing.addObject(Template::getTemplate(req.requestedids(i)));
    }

    bool accessViolation = false;

    for (int i = 0; i < req.requesteduserids_size(); ++i)
    {
      if (req.requesteduserids(i) == User::getLoggedInUserId()) {
        listing.addObjects(Template::getFoodsForUser(req.requesteduserids(i)));
      } else {
        accessViolation = true;
      }
    }

    if (accessViolation) {
      listing.setError("Some requested template names were omitted because they "
                       "belong to another user.");
    }

    return listing;
  }

  StoredTemplateListing storeTemplates
    (const TemplateStoreRequest& req)
  {
    StoredTemplateListing confirmations;
    bool accessViolation = false;
    bool idsMissing = false;
    bool idsInvalid = false;

    for (int i = 0; i < req.templates_size(); ++i) {
      const TemplateData& templData = req.templates(i);

      if (templData.has_id()) {
        QSharedPointer<Template> templ = Template::getTemplate(templData.id());
        UpdateComponents::ComponentModifications cmods;

        if (!templ) {
          templ = Template::createNewTemplate();
        }

        if (templ) {

          if (templ->getOwnerId() == User::getLoggedInUserId()) {

            if (templData.has_name()) {
              templ->setName(QString::fromStdString(templData.name()));
            }

            cmods =
              UpdateComponents::updateComponents(templ, templData.components());

            try {

              templ->saveToDatabase();

              confirmations.addObject(templ);

              confirmations.addComponentModifications
                (templ->getTemplateId(),
                 UpdateComponents::updateComponentModifications(templ, cmods));

              if (templData.id() != templ->getTemplateId()) {
                confirmations.addModification
                  (templData.id(), templ->getTemplateId());
              }

            } catch (const std::exception& ex) {
              confirmations.setError("Failed to store template " + templ->getName() +
                                     " to database. Error was: " +
                                     QString::fromStdString(ex.what()));
            }

          } else {
            accessViolation = true;
          }
        } else {
          idsInvalid = true;
        }
      } else {
        idsMissing = true;
      }
    }

    if (accessViolation) {
      confirmations.setError("Some template data was not stored because it is not "
                             "owned by the currently logged in user.");
    }

    if (idsMissing) {
      confirmations.setError("Some template data was not stored because the request "
                             "contained no ID number");
    }

    if (idsInvalid) {
      confirmations.setError("Some template data was not stored because the "
                             "supplied ID numbers were invalid");
    }

    return confirmations;
  }

  DeletedTemplateListing deleteTemplates
    (const TemplateDeleteRequest& req)
  {
    DeletedTemplateListing confirmations;
    bool accessViolation = false;
    bool idsInvalid = false;

    for (int i = 0; i < req.deleteids_size(); ++i) {

      QSharedPointer<Template> templ = Template::getTemplate(req.deleteids(i));

      if (templ) {

        if (templ->getOwnerId() == User::getLoggedInUserId()) {

          try {
            templ->deleteFromDatabase();
            confirmations.addObject(templ);
          } catch (const std::exception& ex) {
            confirmations.setError("Failed to delete template " + templ->getName() +
                                   " from database. Error was: " +
                                   QString::fromStdString(ex.what()));
          }

        } else {
          accessViolation = true;
        }
      } else {
        idsInvalid = true;
      }
    }

    if (accessViolation) {
      confirmations.setError("Some template data was not deleted because it is not "
                             "owned by the currently logged in user.");
    }

    if (idsInvalid) {
      confirmations.setError("Some template data was not deleted because the "
                             "supplied ID numbers were invalid");
    }

    return confirmations;
  }
}
