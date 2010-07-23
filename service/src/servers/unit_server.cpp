#include "servers/unit_server.h"
#include "servers/data_server.h"
#include "libnutrition/data/unit.h"
#include <cassert>

namespace UnitServer {

  UnitLoadResponseObjects loadUnits(const UnitLoadRequest& req)
  {
    UnitLoadResponseObjects resp_objs;

    if (req.omit()) return resp_objs;

    if (req.all()) {

      resp_objs.addObjects(Unit::getAllUnits());

    } else {

      bool basicUnitsOnly = req.basicunitsonly();

      for (int i = 0; i < req.requesteddimensions_size(); ++i) {
        if (!basicUnitsOnly) {
          resp_objs.addObjects
            (Unit::getAllUnits(static_cast<Unit::Dimensions::Dimension>
              (req.requesteddimensions(i))));
        } else {
          resp_objs.addObject(Unit::getPreferredUnit
                            (static_cast<Unit::Dimensions::Dimension>
                              (req.requesteddimensions(i))));
        }
      }

      for (int i = 0; i < req.requestedabbreviations_size(); ++i) {
        if (!basicUnitsOnly) {
          resp_objs.addObject
            (Unit::getUnit(QString::fromStdString(req.requestedabbreviations(i))));
        } else {
          QSharedPointer<const Unit> unit =
            Unit::getUnit(QString::fromStdString(req.requestedabbreviations(i)));

          if (unit->getConversionFactor() == 1) {
            resp_objs.addObject(unit);
          }
        }
      }

    }

    return resp_objs;
  }
}
