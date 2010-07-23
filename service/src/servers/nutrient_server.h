#ifndef NUTRIENT_SERVER_H_
#define NUTRIENT_SERVER_H_

#include "libnutrition/proto/service/nutrient_messages.pb.h"
#include "libnutrition/data/nutrient.h"
#include "servers/response_objects.h"
#include <QString>
#include <QSet>

class NutrientLoadResponseObjects : public ResponseObjects<Nutrient, NutrientLoadResponse>
{
  protected:

    virtual QString getId
      (const QSharedPointer<const Nutrient>& nutrient) const
        { return nutrient->getId(); }

    virtual void addObjectToResponse
      (NutrientLoadResponse& resp, const QSharedPointer<const Nutrient>& nutrient) const
        { *(resp.add_nutrients()) = nutrient->serialize(); }
};

namespace NutrientServer
{
  NutrientLoadResponseObjects loadNutrients(const NutrientLoadRequest& req);
}

#endif /* NUTRIENT_SERVER_H_ */
