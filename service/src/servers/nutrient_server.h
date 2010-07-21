#ifndef NUTRIENT_SERVER_H_
#define NUTRIENT_SERVER_H_

#include "libnutrition/proto/service/nutrient_messages.pb.h"
#include "libnutrition/data/nutrient.h"
#include <QString>
#include <QSet>

class NutrientLoadResponseObjects
{
  public:

    void addNutrient(const QSharedPointer<const Nutrient>& nutrient);
    void addNutrients(const QVector<QSharedPointer<const Nutrient> >& nutrients);
    void addNutrients(const QList<QSharedPointer<const Nutrient> >& nutrients);

    inline bool isEmpty() const { return nutrients.isEmpty(); }

    QList<QSharedPointer<const Nutrient> > getNutrients() const;

    NutrientLoadResponse serialize() const;

  private:

    QSet<QString> nutrientIds;
    QList<QSharedPointer<const Nutrient> > nutrients;
};

namespace NutrientServer
{
  NutrientLoadResponseObjects loadNutrients(const NutrientLoadRequest& req);
}

#endif /* NUTRIENT_SERVER_H_ */
