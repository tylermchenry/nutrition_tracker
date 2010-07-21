#ifndef NUTRIENT_SERVER_H_
#define NUTRIENT_SERVER_H_

#include "servers/omissions.h"
#include "libnutrition/proto/service/nutrient_messages.pb.h"
#include "libnutrition/data/nutrient.h"
#include <QString>
#include <QSet>

class DataLoadResponseObjects; // forward decl

class NutrientLoadResponseObjects
{
  public:

    void addNutrient(const QSharedPointer<const Nutrient>& nutrient);
    void addNutrients(const QVector<QSharedPointer<const Nutrient> >& nutrients);
    void addNutrients(const QList<QSharedPointer<const Nutrient> >& nutrients);

    QList<QSharedPointer<const Nutrient> > getNutrients() const;

    NutrientLoadResponse serialize() const;

  public:

    QSet<QString> nutrientIds;
    QList<QSharedPointer<const Nutrient> > nutrients;
};

namespace NutrientServer
{
  NutrientLoadResponseObjects loadNutrients
    (const NutrientLoadRequest& req);

  DataLoadResponseObjects& loadNutrients
      (const NutrientLoadRequest& req, DataLoadResponseObjects& resp_objs,
       const Omissions& omissions);
}

#endif /* NUTRIENT_SERVER_H_ */
