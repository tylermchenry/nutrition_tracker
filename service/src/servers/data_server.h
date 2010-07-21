#ifndef DATA_SERVER_H_
#define DATA_SERVER_H_

#include "servers/omissions.h"
#include "servers/single_food_server.h"
#include "servers/composite_food_server.h"
#include "servers/template_server.h"
#include "servers/meal_server.h"
#include "servers/nutrient_server.h"
#include "servers/unit_server.h"
#include "servers/specialized_unit_server.h"
#include "servers/group_server.h"
#include "servers/user_server.h"
#include "libnutrition/proto/service/data_messages.pb.h"

class DataLoadResponseObjects
{
  public:

    explicit DataLoadResponseObjects(const Omissions& omissions)
      : omissions(omissions)
    {}

    GroupLoadResponseObjects group_objects;
    UnitLoadResponseObjects unit_objects;
    NutrientLoadResponseObjects nutrient_objects;

    DataLoadResponse serialize();

  private:

    void acquireDependentObjects();

    Omissions omissions;
};

namespace DataServer {

  DataLoadResponseObjects loadData(const DataLoadRequest& req);

}

#endif /* DATA_SERVER_H_ */
