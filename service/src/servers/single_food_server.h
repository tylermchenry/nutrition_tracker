#ifndef SINGLE_FOOD_SERVER_H_
#define SINGLE_FOOD_SERVER_H_

#include "libnutrition/proto/service/single_food_messages.pb.h"
#include "libnutrition/data/single_food.h"
#include "servers/food_server.h"
#include <QString>
#include <QSet>

namespace SingleFoodServer
{
  FoodLoadResponseObjects loadSingleFoods(const SingleFoodLoadRequest& req);
}

#endif /* SINGLE_FOOD_SERVER_H_ */
