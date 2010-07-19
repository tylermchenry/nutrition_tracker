/*
 * service_back_end_single_food.cpp
 * Part of libnutrition
 *
 *  Created on: Jul 5, 2010
 *      Author: Tyler McHenry <tyler@nerdland.net>
 */

#include "service_back_end.h"
#include "libnutrition/proto/service/data_messages.pb.h"
#include "libnutrition/data/impl/single_food_impl.h"
#include "libnutrition/data/data_cache.h"
#include <stdexcept>

QSharedPointer<SingleFood> ServiceBackEnd::loadSingleFood(int id)
{
  DataLoadRequest req;
  DataLoadResponse resp;
  LoadedData ldata;

  DataCache<SingleFood>::getInstance().remove(id);

  req.mutable_singlefoodloadrequest()->add_requestedids(id);

  setOmissions(req);

  writeMessageAndReadResponse(req, resp);
  loadResponseData(ldata, resp);

  return ldata.singleFoods[id];
}

QMultiMap<QString, int> ServiceBackEnd::loadSingleFoodNamesForUser(int userId)
{
  SingleFoodLoadRequest req;
  SingleFoodLoadResponse resp;

  req.add_requesteduserids(userId);
  req.set_nameandidonly(true);

  writeMessageAndReadResponse(req, resp);

  QMultiMap<QString, int> foodNames;

  for (int i = 0; i < resp.singlefoods_size(); ++i) {
    foodNames.insert(QString::fromStdString(resp.singlefoods(i).name()),
                     resp.singlefoods(i).id());
  }

  return foodNames;
}

void ServiceBackEnd::storeSingleFood(const QSharedPointer<SingleFood>& food)
{
  SingleFoodStoreRequest req;
  SingleFoodStoreResponse resp;

  *(req.add_singlefoods()) = food->serialize();

  writeMessageAndReadResponse(req, resp);

  if (resp.has_error() && resp.error().iserror()) {
    throw std::runtime_error("Single Food store error: " + resp.error().errormessage());
  }

  for (int i = 0; i < resp.storedids_size(); ++i) {
    if (resp.storedids(i) == food->getSingleFoodId()) return;
  }

  throw std::runtime_error("Failed to store Single Food");
}

void ServiceBackEnd::deleteSingleFood(const QSharedPointer<SingleFood>& food)
{
  deleteSingleFood(food->getSingleFoodId());
}

void ServiceBackEnd::deleteSingleFood(int id)
{
  SingleFoodDeleteRequest req;
  SingleFoodDeleteResponse resp;

  req.add_deleteids(id);

  writeMessageAndReadResponse(req, resp);

  if (resp.has_error() && resp.error().iserror()) {
    throw std::runtime_error("Single Food delete error: " + resp.error().errormessage());
  }

  for (int i = 0; i < resp.deletedids_size(); ++i) {
    if (resp.deletedids(i) == id) return;
  }

  throw std::runtime_error("Failed to delete Single Food");
}


void ServiceBackEnd::loadResponseData
  (LoadedData& loadedData, const SingleFoodLoadResponse& resp)
{
  if (resp.has_error() && resp.error().iserror()) {
    throw std::runtime_error("Single Food load error: " + resp.error().errormessage());
  }

  for (int i = 0; i < resp.singlefoods_size(); ++i) {

    const SingleFoodData& sfdata = resp.singlefoods(i);
    int id = sfdata.id();

    QSharedPointer<SingleFood> singleFood =
      DataCache<SingleFood>::getInstance().get(id);

    if (!singleFood) {

      QMap<QString, NutrientAmount> nutrients;

      for (int j = 0; j < sfdata.nutrientamounts_size(); ++j) {
        const SingleFoodData::NutrientAmount& nadata = sfdata.nutrientamounts(j);

        NutrientAmount nutrAmount
          (Nutrient::getNutrient(QString::fromStdString(nadata.nutrientid())),
           nadata.amount());

        nutrients.insert(QString::fromStdString(nadata.nutrientid()), nutrAmount);
      }

      // For simplicity, there is a source invariant that the values of the
      // enumeration elements in the protocol buffers match the values of the
      // elements in the corresponding enumeration inside a data class.

      singleFood = QSharedPointer<SingleFood>
        (new SingleFoodImpl
          (id,
           QString::fromStdString(sfdata.name()),
           sfdata.ownerid(),
           static_cast<SingleFood::EntrySources::EntrySource>(sfdata.entrysource()),
           Group::getGroup(QString::fromStdString(sfdata.groupid())),
           sfdata.percentrefuse(),
           QString::fromStdString(sfdata.refusedescription()),
           nutrients,
           sfdata.weightamount(), sfdata.volumeamount(),
           sfdata.quantityamount(), sfdata.servingamount(),
           sfdata.energydensityfat(), sfdata.energydensitycarbohydrate(),
           sfdata.energydensityprotein(), sfdata.energydensityalcohol()));

      DataCache<SingleFood>::getInstance().insert(id, singleFood);
    }

    loadedData.singleFoods.insert(id, singleFood);
  }
}
