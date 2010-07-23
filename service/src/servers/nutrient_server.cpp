#include "servers/nutrient_server.h"
#include "servers/data_server.h"
#include "libnutrition/data/nutrient.h"
#include <cassert>

namespace NutrientServer {

  NutrientLoadResponseObjects loadNutrients(const NutrientLoadRequest& req)
  {
    NutrientLoadResponseObjects resp_objs;

    if (req.omit()) return resp_objs;

    if (req.all()) {

      resp_objs.addObjects(Nutrient::getAllNutrients());

    } else {

      for (int i = 0; i < req.requestedids_size(); ++i) {
        resp_objs.addObject
          (Nutrient::getNutrient(QString::fromStdString(req.requestedids(i))));
      }

      for (int i = 0; i < req.requestednames_size(); ++i) {
        resp_objs.addObject
          (Nutrient::getNutrientByName(QString::fromStdString(req.requestednames(i))));
      }

      for (int i = 0; i < req.requestedcategories_size(); ++i) {
        resp_objs.addObjects
          (Nutrient::getAllNutrients(static_cast<Nutrient::Categories::Category>
            (req.requestedcategories(i))));
      }
    }

    return resp_objs;
  }
}
