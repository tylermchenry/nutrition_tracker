#ifndef UNIT_SERVER_H_
#define UNIT_SERVER_H_

#include "libnutrition/proto/service/unit_messages.pb.h"
#include "libnutrition/data/unit.h"
#include "servers/response_objects.h"
#include <QString>
#include <QSet>

class UnitLoadResponseObjects : public ResponseObjects<Unit, UnitLoadResponse>
{
  protected:

    virtual QString getId
      (const QSharedPointer<const Unit>& unit) const
        { return unit->getAbbreviation(); }

    virtual void addObjectToResponse
      (UnitLoadResponse& resp, const QSharedPointer<const Unit>& unit) const
        { *(resp.add_units()) = unit->serialize(); }
};

namespace UnitServer
{
  UnitLoadResponseObjects loadUnits(const UnitLoadRequest& req);
}

#endif /* UNIT_SERVER_H_ */
