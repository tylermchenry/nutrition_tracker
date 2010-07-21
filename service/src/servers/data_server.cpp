#include "servers/data_server.h"
#include "servers/omissions.h"

DataLoadResponse DataLoadResponseObjects::serialize() const
{
  DataLoadResponse resp;

  *(resp.mutable_nutrientloadresponse()) = nutrient_objects.serialize();

  return resp;
}

namespace DataServer {

  DataLoadResponseObjects loadData(const DataLoadRequest& req)
  {
    DataLoadResponseObjects resp_objs;
    Omissions omissions(req);

    if (req.has_nutrientloadrequest()) {
      NutrientServer::loadNutrients(req.nutrientloadrequest(), resp_objs, omissions);
    }

    return resp_objs;
  }

}
