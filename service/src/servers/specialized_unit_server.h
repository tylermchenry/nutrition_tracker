#ifndef SPECIALIZED_UNIT_SERVER_H_
#define SPECIALIZED_UNIT_SERVER_H_

#include "libnutrition/proto/service/specialized_unit_messages.pb.h"
#include "libnutrition/data/specialized_unit.h"
#include "servers/response_objects.h"
#include <QString>
#include <QSet>

class SpecializedUnitLoadResponseObjects
  : public ResponseObjects<SpecializedUnit, SpecializedUnitLoadResponse>
{
  protected:

    virtual SpecializedUnit::SpecializedUnitIdTuple getId
      (const QSharedPointer<const SpecializedUnit>& specializedUnit) const
        { return specializedUnit->getSpecializedUnitIdTuple(); }

    virtual void addObjectToResponse
      (SpecializedUnitLoadResponse& resp,
       const QSharedPointer<const SpecializedUnit>& specializedUnit) const
        { *(resp.add_specializedunits()) = specializedUnit->serialize(); }
};

namespace SpecializedUnitServer
{
  SpecializedUnitLoadResponseObjects loadSpecializedUnits
    (const SpecializedUnitLoadRequest& req);
}

#endif /* SPECIALIZED_UNIT_SERVER_H_ */
