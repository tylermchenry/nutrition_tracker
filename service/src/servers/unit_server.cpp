#include "servers/unit_server.h"
#include "servers/data_server.h"
#include "libnutrition/data/unit.h"
#include <cassert>

void UnitLoadResponseObjects::addUnit
(const QSharedPointer<const Unit>& unit)
{
  if (unit && !unitIds.contains(unit->getAbbreviation())) {
    unitIds.insert(unit->getAbbreviation());
    units.append(unit);
  }
}

void UnitLoadResponseObjects::addUnits
  (const QVector<QSharedPointer<const Unit> >& units)
{
  for (QVector<QSharedPointer<const Unit> >::const_iterator i = units.begin();
       i != units.end(); ++i)
  {
    addUnit(*i);
  }
}

void UnitLoadResponseObjects::addUnits
  (const QList<QSharedPointer<const Unit> >& units)
{
  for (QList<QSharedPointer<const Unit> >::const_iterator i = units.begin();
       i != units.end(); ++i)
  {
    addUnit(*i);
  }
}

QList<QSharedPointer<const Unit> > UnitLoadResponseObjects::getUnits() const
{
  return units;
}

UnitLoadResponse UnitLoadResponseObjects::serialize() const
{
  UnitLoadResponse resp;

  for (QList<QSharedPointer<const Unit> >::const_iterator i = units.begin();
       i != units.end(); ++i)
  {
    *(resp.add_units()) = (*i)->serialize();
  }

  return resp;
}

namespace UnitServer {

  UnitLoadResponseObjects loadUnits
    (const UnitLoadRequest& req)
  {
    UnitLoadResponseObjects resp_objs;

    if (req.omit()) return resp_objs;

    if (req.all()) {

      resp_objs.addUnits(Unit::getAllUnits());

    } else {

      bool basicUnitsOnly = req.basicunitsonly();

      for (int i = 0; i < req.requesteddimensions_size(); ++i) {
        if (!basicUnitsOnly) {
          resp_objs.addUnits
            (Unit::getAllUnits(static_cast<Unit::Dimensions::Dimension>
              (req.requesteddimensions(i))));
        } else {
          resp_objs.addUnit(Unit::getPreferredUnit
                            (static_cast<Unit::Dimensions::Dimension>
                              (req.requesteddimensions(i))));
        }
      }

      for (int i = 0; i < req.requestedabbreviations_size(); ++i) {
        if (!basicUnitsOnly) {
          resp_objs.addUnit
            (Unit::getUnit(QString::fromStdString(req.requestedabbreviations(i))));
        } else {
          QSharedPointer<const Unit> unit =
            Unit::getUnit(QString::fromStdString(req.requestedabbreviations(i)));

          if (unit->getConversionFactor() == 1) {
            resp_objs.addUnit(unit);
          }
        }
      }

    }

    return resp_objs;
  }

  DataLoadResponseObjects& loadUnits
    (const UnitLoadRequest& req, DataLoadResponseObjects& resp_objs,
     const Omissions&)
  {
    QList<QSharedPointer<const Unit> > units =
      loadUnits(req).getUnits();

    resp_objs.unit_objects.addUnits(units);

    return resp_objs;
  }
}
