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

    if (req.requestedids_size() > 0) {
      resp_objs.setError(BackEnd::FoodTypes::Template,
                         "Requesting all templates for particular user(s) "
                         "without the nameAndIdOnly option is not currently "
                         "supported");
    }

    return resp_objs;
  }

  TemplateListing loadTemplateNames
    (const TemplateLoadRequest& req, int loggedInUserId)
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
      if (req.requesteduserids(i) == loggedInUserId) {
        listing.addObjects(Template::getFoodsForUser(req.requesteduserids(i)));
      } else {
        accessViolation = true;
      }
    }

    if (accessViolation) {
      listing.setError("Some requested food names were omitted because they "
                       "belong to another user.");
    }

    return listing;
  }
}
