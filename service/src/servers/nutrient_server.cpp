#include "servers/nutrient_server.h"
#include "servers/data_server.h"
#include "libnutrition/data/nutrient.h"
#include <cassert>

void NutrientLoadResponseObjects::addNutrient
(const QSharedPointer<const Nutrient>& nutrient)
{
  if (nutrient && !nutrientIds.contains(nutrient->getId())) {
    nutrientIds.insert(nutrient->getId());
    nutrients.append(nutrient);
  }
}

void NutrientLoadResponseObjects::addNutrients
  (const QVector<QSharedPointer<const Nutrient> >& nutrients)
{
  for (QVector<QSharedPointer<const Nutrient> >::const_iterator i = nutrients.begin();
       i != nutrients.end(); ++i)
  {
    addNutrient(*i);
  }
}

void NutrientLoadResponseObjects::addNutrients
  (const QList<QSharedPointer<const Nutrient> >& nutrients)
{
  for (QList<QSharedPointer<const Nutrient> >::const_iterator i = nutrients.begin();
       i != nutrients.end(); ++i)
  {
    addNutrient(*i);
  }
}

QList<QSharedPointer<const Nutrient> > NutrientLoadResponseObjects::getNutrients() const
{
  return nutrients;
}

NutrientLoadResponse NutrientLoadResponseObjects::serialize() const
{
  NutrientLoadResponse resp;

  for (QList<QSharedPointer<const Nutrient> >::const_iterator i = nutrients.begin();
       i != nutrients.end(); ++i)
  {
    *(resp.add_nutrients()) = (*i)->serialize();
  }

  return resp;
}

namespace NutrientServer {

  NutrientLoadResponseObjects loadNutrients
    (const NutrientLoadRequest& req)
  {
    NutrientLoadResponseObjects resp_objs;

    if (req.omit()) return resp_objs;

    if (req.all()) {

      resp_objs.addNutrients(Nutrient::getAllNutrients());

    } else {

      for (int i = 0; i < req.requestedids_size(); ++i) {
        resp_objs.addNutrient
          (Nutrient::getNutrient(QString::fromStdString(req.requestedids(i))));
      }

      for (int i = 0; i < req.requestednames_size(); ++i) {
        resp_objs.addNutrient
          (Nutrient::getNutrientByName(QString::fromStdString(req.requestednames(i))));
      }

      for (int i = 0; i < req.requestedcategories_size(); ++i) {
        resp_objs.addNutrients
          (Nutrient::getAllNutrients(static_cast<Nutrient::Categories::Category>
            (req.requestedcategories(i))));
      }
    }

    return resp_objs;
  }

  DataLoadResponseObjects& loadNutrients
    (const NutrientLoadRequest& req, DataLoadResponseObjects& resp_objs,
     const Omissions& omissions)
  {
    QList<QSharedPointer<const Nutrient> > nutrients =
      loadNutrients(req).getNutrients();

    resp_objs.nutrient_objects.addNutrients(nutrients);

    if (!omissions.units) {
      for (QList<QSharedPointer<const Nutrient> >::const_iterator i = nutrients.begin();
           i != nutrients.end(); ++i)
      {
        resp_objs.unit_objects.addUnit((*i)->getStandardUnit());
      }
    }

    return resp_objs;
  }
}
