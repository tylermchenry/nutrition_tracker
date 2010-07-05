/*
 * service_back_end_composite_food.cpp
 * Part of libnutrition
 *
 *  Created on: Jul 5, 2010
 *      Author: Tyler McHenry <tyler@nerdland.net>
 */

#include "service_back_end.h"
#include "libnutrition/proto/data.pb.h"
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

  req.add_requestedids(userId);
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

      setComponents(compositeFood_impl, cfdata.components());

      compositeFood = compositeFood_impl;
      DataCache<CompositeFood>::getInstance().insert(id, compositeFood);
    }

    loadedData.compositeFoods.insert(id, compositeFood);
  }
}
