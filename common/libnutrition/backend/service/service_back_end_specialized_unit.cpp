/*
 * service_back_end_specialized_specializedUnit.cpp
 * Part of libnutrition
 *
 *  Created on: Jul 4, 2010
 *      Author: Tyler McHenry <tyler@nerdland.net>
 */

#include "service_back_end.h"
#include "libnutrition/proto/service/data_messages.pb.h"
#include "libnutrition/proto/service/specialized_unit_messages.pb.h"
#include "libnutrition/data/impl/specialized_unit_impl.h"
#include "libnutrition/data/data_cache.h"
#include <stdexcept>

QSharedPointer<const SpecializedUnit> ServiceBackEnd::loadSpecializedUnit
  (int foodId, int sequence)
{
  SpecializedUnitLoadRequest req;
  SpecializedUnitLoadResponse resp;
  LoadedData ldata;

  SpecializedUnitIdentifier* id = req.add_requestedids();

  id->set_singlefoodid(foodId);
  id->set_sequence(sequence);

  writeMessageAndReadResponse(req, resp);
  loadResponseData(ldata, resp);

  return ldata.specializedUnits
    [SpecializedUnit::SpecializedUnitIdTuple(foodId, sequence)];
}

QList<QSharedPointer<SpecializedUnit> >
  ServiceBackEnd::loadAllSpecializedUnitsForFood(int foodId)
{
  SpecializedUnitLoadRequest req;
  SpecializedUnitLoadResponse resp;
  LoadedData ldata;

  req.add_requestedsinglefoodids(foodId);

  writeMessageAndReadResponse(req, resp);
  loadResponseData(ldata, resp);

  return ldata.specializedUnits.values();
}

QPair<QList<QSharedPointer<Unit> >, QList<QSharedPointer<SpecializedUnit> > >
ServiceBackEnd::loadAllUnitsForFood
   (int foodId)
{
  DataLoadRequest req;
  DataLoadResponse resp;
  LoadedData ldata;

  req.mutable_specializedunitloadrequest()->add_requestedsinglefoodids(foodId);

  if (!loadedAllUnits) {
    // TODO: Perhaps restrict to dimensions that this food can be measured in?
    req.mutable_unitloadrequest()->set_all(true);
  }

  setOmissions(req);

  writeMessageAndReadResponse(req, resp);
  loadResponseData(ldata, resp);

  return qMakePair(ldata.units.values(), ldata.specializedUnits.values());
}

void ServiceBackEnd::loadResponseData
  (LoadedData& loadedData, const SpecializedUnitLoadResponse& resp)
{
  if (resp.has_error() && resp.error().iserror()) {
    throw std::runtime_error("Specialized Unit load error: " + resp.error().errormessage());
  }

  for (int i = 0; i < resp.specializedunits_size(); ++i) {

    const SpecializedUnitData& sudata = resp.specializedunits(i);
    SpecializedUnit::SpecializedUnitIdTuple idTuple =
      SpecializedUnit::SpecializedUnitIdTuple(sudata.foodid(), sudata.sequence());

    // For simplicity, there is a source invariant that the values of the
    // enumeration elements in the protocol buffers match the values of the
    // elements in the corresponding enumeration inside a data class.

    QSharedPointer<SpecializedUnit> specializedUnit = QSharedPointer<SpecializedUnit>
      (new SpecializedUnitImpl
         (sudata.foodid(), sudata.sequence(),
          QString::fromStdString(sudata.name()),
          sudata.amountingrams()));

    DataCache<SpecializedUnit>::getInstance().insert(idTuple, specializedUnit);
    loadedData.specializedUnits.insert(idTuple, specializedUnit);
  }
}
