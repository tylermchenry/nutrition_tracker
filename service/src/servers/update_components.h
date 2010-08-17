/*
 * update_components.h
 *
 *  Created on: Aug 17, 2010
 *      Author: tyler
 */

#ifndef UPDATE_COMPONENTS_H_
#define UPDATE_COMPONENTS_H_

#include <libnutrition/data/food_collection.h>
#include <libnutrition/proto/data/food_component.pb.h>
#include <libnutrition/proto/service/composite_food_messages.pb.h>
#include <QSharedPointer>
#include <QMap>
#include <QPair>

namespace UpdateComponents {

  struct ComponentModifications
  {
    QMap<int, int> idAssignments; // New Id -> Old Id
    QMap<int, QPair<int, int> > orderChanges; // New Id -> (Old Order, New Order)
  };

  ComponentModifications updateComponents
    (const QSharedPointer<FoodCollection>& collection,
     const google::protobuf::RepeatedPtrField<FoodComponentData>& components);

  template <typename R>
  R& addComponentModificationsToResponse
    (R& response, int collectionId, const ComponentModifications& modifications)
  {
    QSet<int> modifiedIds = modifications.idAssignments.keys().toSet();
    modifiedIds.unite(modifications.orderChanges.keys().toSet());

    for (QSet<int>::const_iterator i = modifiedIds.begin();
         i != modifiedIds.end(); ++i)
    {
      ::ComponentModification* cmod = response.add_componentmodifications();

      cmod->set_collectionid(collectionId);

      if (modifications.idAssignments.contains(*i)) {
        cmod->set_oldcomponentid(modifications.idAssignments(*i));
      } else {
        cmod->set_oldcomponentid(*i); // Set anyway for easy client lookup
      }

      cmod->set_newcomponentid(*i); // Always set this for consistency

      if (modifications.orderChanges.contains(*i)) {
        cmod->set_oldorder(modifications.orderChanges[*i].first);
        cmod->set_neworder(modifications.orderChanges[*i].second);
      }
    }

    return response;
  }
}

#endif /* UPDATE_COMPONENTS_H_ */
