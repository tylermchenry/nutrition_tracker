/*
 * service_back_end_unit.cpp
 * Part of libnutrition
 *
 *  Created on: Jul 4, 2010
 *      Author: Tyler McHenry <tyler@nerdland.net>
 */

#include "service_back_end.h"
#include "libnutrition/proto/service/unit_messages.pb.h"
#include "libnutrition/data/impl/unit_impl.h"
#include "libnutrition/data/data_cache.h"
#include <stdexcept>

QSharedPointer<const Unit> ServiceBackEnd::loadUnit(const QString& abbr)
{
  UnitLoadRequest req;
  UnitLoadResponse resp;
  LoadedData ldata;

  req.add_requestedabbreviations(abbr.toStdString());

  writeMessageAndReadResponse(req, resp);
  loadResponseData(ldata, resp);

  return ldata.units[abbr];
}

QList<QSharedPointer<Unit> > ServiceBackEnd::loadAllUnits()
{
  UnitLoadRequest req;
  UnitLoadResponse resp;
  LoadedData ldata;

  req.set_all(true);

  writeMessageAndReadResponse(req, resp);
  loadResponseData(ldata, resp);

  QList<QSharedPointer<Unit> > all = ldata.units.values();

  // We will instruct server to omit groups on future data requests
  loadedAllUnits = true;

  return all;
}

QList<QSharedPointer<Unit> > ServiceBackEnd::loadAllUnits
   (Unit::Dimensions::Dimension dimension)
{
  UnitLoadRequest req;
  UnitLoadResponse resp;
  LoadedData ldata;

  req.add_requesteddimensions(static_cast<UnitLoadRequest_Dimension>(dimension));

  writeMessageAndReadResponse(req, resp);
  loadResponseData(ldata, resp);

  return ldata.units.values();
}

QSharedPointer<const Unit> ServiceBackEnd::loadBasicUnit
   (Unit::Dimensions::Dimension dimension)
{
  UnitLoadRequest req;
  UnitLoadResponse resp;
  LoadedData ldata;

  req.add_requesteddimensions(static_cast<UnitLoadRequest_Dimension>(dimension));
  req.set_basicunitsonly(true);

  writeMessageAndReadResponse(req, resp);
  loadResponseData(ldata, resp);

  for (QMap<QString, QSharedPointer<Unit> >::const_iterator i = ldata.units.begin();
      i != ldata.units.end(); ++i)
  {
    if (i.value()->getDimension() == dimension) {
      return i.value();
    }
  }

  return QSharedPointer<const Unit>();
}

void ServiceBackEnd::loadResponseData
  (LoadedData& loadedData, const UnitLoadResponse& resp)
{
  if (resp.has_error() && resp.error().iserror()) {
    throw std::runtime_error("Unit load error: " + resp.error().errormessage());
  }

  for (int i = 0; i < resp.units_size(); ++i) {

    const UnitData& udata = resp.units(i);
    QString abbr = QString::fromStdString(udata.abbreviation());

    // For simplicity, there is a source invariant that the values of the
    // enumeration elements in the protocol buffers match the values of the
    // elements in the corresponding enumeration inside a data class.

    QSharedPointer<Unit> unit = QSharedPointer<Unit>
      (new UnitImpl
         (abbr,
          QString::fromStdString(udata.name()),
          static_cast<Unit::Dimensions::Dimension>(udata.dimension()),
          udata.basicconversionfactor()));

    DataCache<Unit>::getInstance().insert(abbr, unit);
    loadedData.units.insert(abbr, unit);
  }
}
