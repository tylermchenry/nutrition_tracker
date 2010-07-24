#include "servers/specialized_unit_server.h"
#include "servers/data_server.h"
#include "libnutrition/data/specialized_unit.h"
#include <cassert>

namespace SpecializedUnitServer {

  SpecializedUnitLoadResponseObjects loadSpecializedUnits
    (const SpecializedUnitLoadRequest& req)
  {
    SpecializedUnitLoadResponseObjects resp_objs;

    if (req.omit()) return resp_objs;

    for (int i = 0; i < req.requestedsinglefoodids_size(); ++i) {
      resp_objs.addObjects
        (SpecializedUnit::getAllSpecializedUnitsForFoodId
          (req.requestedsinglefoodids(i)));
    }

    for (int i = 0; i < req.requestedids_size(); ++i) {
      const SpecializedUnitIdentifier& id = req.requestedids(i);
      resp_objs.addObject
        (SpecializedUnit::getSpecializedUnit(id.singlefoodid(), id.sequence()));
    }

    return resp_objs;
  }
}
