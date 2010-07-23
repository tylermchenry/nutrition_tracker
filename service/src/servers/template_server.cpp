#include "servers/template_server.h"

namespace TemplateServer {

  FoodLoadResponseObjects loadTemplates(const TemplateLoadRequest& req)
  {
    FoodLoadResponseObjects resp_objs;

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
}
