/*
 * service_back_end_nutrient.cpp
 * Part of libnutrition
 *
 *  Created on: Jul 4, 2010
 *      Author: Tyler McHenry <tyler@nerdland.net>
 */

#include "service_back_end.h"
#include "libnutrition/proto/data.pb.h"
#include "libnutrition/data/impl/nutrient_impl.h"
#include "libnutrition/data/data_cache.h"
#include <stdexcept>

QSharedPointer<const Nutrient> ServiceBackEnd::loadNutrient(const QString& id)
{
  DataLoadRequest req;
  DataLoadResponse resp;
  LoadedData ldata;

  req.mutable_nutrientloadrequest()->add_requestedids(id.toStdString());
  setOmissions(req);

  writeMessageAndReadResponse(req, resp);
  loadResponseData(ldata, resp);

  return ldata.nutrients[id];
}

QList<QSharedPointer<Nutrient> > ServiceBackEnd::loadAllNutrients()
{
  DataLoadRequest req;
  DataLoadResponse resp;
  LoadedData ldata;

  req.mutable_nutrientloadrequest()->set_all(true);
  setOmissions(req);

  writeMessageAndReadResponse(req, resp);
  loadResponseData(ldata, resp);

  QList<QSharedPointer<Nutrient> > all = ldata.nutrients.values();

  // We will instruct server to omit nutrients on future data requests
  loadedAllNutrients = true;

  return all;
}

void ServiceBackEnd::loadResponseData
  (LoadedData& loadedData, const NutrientLoadResponse& resp)
{
  if (resp.has_error() && resp.error().iserror()) {
    throw std::runtime_error("Nutrient load error: " + resp.error().errormessage());
  }

  for (int i = 0; i < resp.nutrients_size(); ++i) {

    const NutrientData& ndata = resp.nutrients(i);
    QString id = QString::fromStdString(ndata.id());

    QSharedPointer<Nutrient> nutrient = QSharedPointer<Nutrient>
      (new NutrientImpl
         (id,
          QString::fromStdString(ndata.name()),
          static_cast<Nutrient::Categories::Category>(ndata.category()),
          Unit::getUnit(QString::fromStdString(ndata.standardunitabbreviation())),
          ndata.rdi()));

    DataCache<Nutrient>::getInstance().insert(id, nutrient);
    loadedData.nutrients.insert(id, nutrient);
  }
}
