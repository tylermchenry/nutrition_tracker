#ifndef COMPOSITE_FOOD_SERVER_H_
#define COMPOSITE_FOOD_SERVER_H_

#include "libnutrition/proto/service/composite_food_messages.pb.h"
#include "libnutrition/data/composite_food.h"
#include "servers/food_server.h"
#include <QString>
#include <QSet>

namespace CompositeFoodServer
{
  FoodLoadResponseObjects loadCompositeFoods(const CompositeFoodLoadRequest& req);

  FoodLoadResponseObjects& loadCompositeFoods
    (FoodLoadResponseObjects& resp_objs, const CompositeFoodLoadRequest& req);
}

#endif /* COMPOSITE_FOOD_SERVER_H_ */
