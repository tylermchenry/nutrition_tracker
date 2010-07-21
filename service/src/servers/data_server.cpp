#include "servers/data_server.h"
#include "servers/omissions.h"

DataLoadResponse DataLoadResponseObjects::serialize()
{
  DataLoadResponse resp;

  acquireDependentObjects();

  if (!omissions.groups && !group_objects.isEmpty()) {
    *(resp.mutable_grouploadresponse()) = group_objects.serialize();
  }

  if (!omissions.units && !unit_objects.isEmpty()) {
    *(resp.mutable_unitloadresponse()) = unit_objects.serialize();
  }

  if (!omissions.nutrients && !nutrient_objects.isEmpty()) {
    *(resp.mutable_nutrientloadresponse()) = nutrient_objects.serialize();
  }

  return resp;
}

void DataLoadResponseObjects::acquireDependentObjects()
{
  // Meals acquire Foods (singles, composites, templates)

  // Foods recursively acquire other Foods

  // Foods acquire Nutrients

  // Nutrients and Specialized Units acquire Units

  if (!omissions.units) {
    if (!omissions.nutrients) {

      QList<QSharedPointer<const Nutrient> > nutrients = nutrient_objects.getNutrients();

      for (QList<QSharedPointer<const Nutrient> >::const_iterator i = nutrients.begin();
           i != nutrients.end(); ++i)
      {
        unit_objects.addUnit((*i)->getStandardUnit());
      }
    }
  }
}

namespace DataServer {

  DataLoadResponseObjects loadData(const DataLoadRequest& req)
  {
    DataLoadResponseObjects resp_objs((Omissions(req)));

    if (req.has_grouploadrequest()) {
      resp_objs.group_objects = GroupServer::loadGroups(req.grouploadrequest());
    }

    if (req.has_unitloadrequest()) {
      resp_objs.unit_objects = UnitServer::loadUnits(req.unitloadrequest());
    }

    if (req.has_nutrientloadrequest()) {
      resp_objs.nutrient_objects = NutrientServer::loadNutrients(req.nutrientloadrequest());
    }

    return resp_objs;
  }

}
