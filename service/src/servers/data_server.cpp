#include "servers/data_server.h"
#include "servers/omissions.h"

DataLoadResponse DataLoadResponseObjects::serialize() const
{
  DataLoadResponse resp;

  if (!group_objects.isEmpty()) {
    *(resp.mutable_grouploadresponse()) = group_objects.serialize();
  }

  if (!unit_objects.isEmpty()) {
    *(resp.mutable_unitloadresponse()) = unit_objects.serialize();
  }

  if (!nutrient_objects.isEmpty()) {
    *(resp.mutable_nutrientloadresponse()) = nutrient_objects.serialize();
  }

  return resp;
}

namespace DataServer {

  DataLoadResponseObjects loadData(const DataLoadRequest& req)
  {
    DataLoadResponseObjects resp_objs;
    Omissions omissions(req);

    if (req.has_grouploadrequest()) {
      GroupServer::loadGroups(req.grouploadrequest(), resp_objs, omissions);
    }

    if (req.has_unitloadrequest()) {
      UnitServer::loadUnits(req.unitloadrequest(), resp_objs, omissions);
    }

    if (req.has_nutrientloadrequest()) {
      NutrientServer::loadNutrients(req.nutrientloadrequest(), resp_objs, omissions);
    }

    return resp_objs;
  }

}
