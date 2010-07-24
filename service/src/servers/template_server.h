#ifndef TEMPLATE_SERVER_H_
#define TEMPLATE_SERVER_H_

#include "libnutrition/proto/service/template_messages.pb.h"
#include "libnutrition/data/template.h"
#include "servers/food_server.h"
#include <QString>
#include <QSet>

namespace TemplateServer
{
  FoodLoadResponseObjects loadTemplates(const TemplateLoadRequest& req);

  FoodLoadResponseObjects& loadTemplates
    (FoodLoadResponseObjects& resp_objs, const TemplateLoadRequest& req);
}

#endif /* TEMPLATE_SERVER_H_ */
