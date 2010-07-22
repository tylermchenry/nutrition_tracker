/*
 * service_back_end_composite_food.cpp
 * Part of libnutrition
 *
 *  Created on: Jul 5, 2010
 *      Author: Tyler McHenry <tyler@nerdland.net>
 */

#include "service_back_end.h"
#include "libnutrition/proto/service/data_messages.pb.h"
#include "libnutrition/data/impl/composite_food_impl.h"
#include "libnutrition/data/data_cache.h"
#include <stdexcept>

QSharedPointer<CompositeFood> ServiceBackEnd::loadCompositeFood(int id)
{
  DataLoadRequest req;
  DataLoadResponse resp;
  LoadedData ldata;

  DataCache<CompositeFood>::getInstance().remove(id);

  req.mutable_compositefoodloadrequest()->add_requestedids(id);

  setOmissions(req);

  writeMessageAndReadResponse(req, resp);
  loadResponseData(ldata, resp);

  return ldata.compositeFoods[id];
}

QMultiMap<QString, int> ServiceBackEnd::loadCompositeFoodNamesForUser
  (int userId, bool includeExpired)
{
  CompositeFoodLoadRequest req;
  CompositeFoodLoadResponse resp;

  req.add_requesteduserids(userId);
  req.set_omitexpired(!includeExpired);
  req.set_nameandidonly(true);

  writeMessageAndReadResponse(req, resp);

  QMultiMap<QString, int> foodNames;

  for (int i = 0; i < resp.compositefoods_size(); ++i) {
    foodNames.insert(QString::fromStdString(resp.compositefoods(i).name()),
                     resp.compositefoods(i).id());
  }

  return foodNames;
}

void ServiceBackEnd::storeCompositeFood(const QSharedPointer<CompositeFood>& food)
{
  CompositeFoodStoreRequest req;
  CompositeFoodStoreResponse resp;

  *(req.add_compositefoods()) = food->serialize();

  writeMessageAndReadResponse(req, resp);

  if (resp.has_error() && resp.error().iserror()) {
    throw std::runtime_error("Composite Food store error: " + resp.error().errormessage());
  }

  for (int i = 0; i < resp.storedids_size(); ++i) {
    if (resp.storedids(i) == food->getCompositeFoodId()) return;
  }

  throw std::runtime_error("Failed to store Composite Food");
}

void ServiceBackEnd::deleteCompositeFood(const QSharedPointer<CompositeFood>& food)
{
  deleteCompositeFood(food->getCompositeFoodId());
}

void ServiceBackEnd::deleteCompositeFood(int id)
{
  CompositeFoodDeleteRequest req;
  CompositeFoodDeleteResponse resp;

  req.add_deleteids(id);

  writeMessageAndReadResponse(req, resp);

  if (resp.has_error() && resp.error().iserror()) {
    throw std::runtime_error("Composite Food delete error: " + resp.error().errormessage());
  }

  for (int i = 0; i < resp.deletedids_size(); ++i) {
    if (resp.deletedids(i) == id) return;
  }

  throw std::runtime_error("Failed to delete Composite Food");
}

void ServiceBackEnd::loadResponseData
  (LoadedData& loadedData, const CompositeFoodLoadResponse& resp)
{
  if (resp.has_error() && resp.error().iserror()) {
    throw std::runtime_error("Composite Food load error: " + resp.error().errormessage());
  }

  for (int i = 0; i < resp.compositefoods_size(); ++i) {

    const CompositeFoodData& cfdata = resp.compositefoods(i);
    int id = cfdata.id();

    QSharedPointer<CompositeFood> compositeFood =
      DataCache<CompositeFood>::getInstance().get(id);

    if (!compositeFood) {

      // For simplicity, there is a source invariant that the values of the
      // enumeration elements in the protocol buffers match the values of the
      // elements in the corresponding enumeration inside a data class.

      QSharedPointer<CompositeFoodImpl> compositeFood_impl;

      compositeFood_impl = QSharedPointer<CompositeFoodImpl>
        (new CompositeFoodImpl
          (id,
           QString::fromStdString(cfdata.name()),
           cfdata.ownerid(),
           cfdata.weightamount(), cfdata.volumeamount(),
           cfdata.quantityamount(), cfdata.servingamount(),
           QDate::fromString(QString::fromStdString(cfdata.creationdate_iso8601()), Qt::ISODate),
           QDate::fromString(QString::fromStdString(cfdata.expirydate_iso8601()), Qt::ISODate)));

      // Must add to cache before setting components, to get canonical pointers
      compositeFood = compositeFood_impl;
      DataCache<CompositeFood>::getInstance().insert(id, compositeFood);

      setComponents(compositeFood_impl, cfdata.components());
    }

    loadedData.compositeFoods.insert(id, compositeFood);
  }
}
