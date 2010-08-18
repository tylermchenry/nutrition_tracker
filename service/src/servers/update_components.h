/*
 * update_components.h
 *
 *  Created on: Aug 17, 2010
 *      Author: tyler
 */

#ifndef UPDATE_COMPONENTS_H_
#define UPDATE_COMPONENTS_H_

#include "response_objects.h"
#include <libnutrition/data/food_collection.h>
#include <libnutrition/proto/data/food_component.pb.h>
#include <libnutrition/proto/service/composite_food_messages.pb.h>
#include <QSharedPointer>
#include <QString>
#include <QMap>
#include <QPair>

namespace UpdateComponents {

  struct ComponentModifications
  {
    QMap<int, int> idAssignments; // New Id -> Old Id
    QMap<int, QPair<int, int> > orderChanges; // New Id -> (Old Order, New Order)

    ComponentModifications();

    void setError(const QString& errorMessage);
    bool isError() const;
    QString getErrorMessage() const;

    private:

      bool bIsError;
      QString errorMessage;
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
        cmod->set_oldcomponentid(modifications.idAssignments[*i]);
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

template <typename K, typename R>
class ComponentModificationListing
{
  public:

    typedef UpdateComponents::ComponentModifications ComponentModifications;

    void addModifications(const K& id, const ComponentModifications& mods)
    {
      if (mods.isError()) {
        respObjs.setError(mods.getErrorMessage());
      }

      return respObjs.addObject(qMakePair(id, mods));
    }

    virtual R serialize() const
       { return respObjs.serialize(); }

    virtual R serialize(R& resp) const
       { return respObjs.serialize(resp); }

  private:

    class CMLResponseObjects
      : public ResponseObjects<QPair<K, ComponentModifications>,
                                R, K, QPair<K, ComponentModifications> >
      {
        protected:

          virtual bool isValid (const QPair<K, ComponentModifications>&) const
            { return true; }

          virtual K getId (const QPair<K, ComponentModifications>& obj) const
            { return obj.first; }

          virtual ComponentModifications getName
            (const QPair<K, ComponentModifications>& obj) const
            { return obj.second; }

          virtual void addObjectToResponse
            (R& resp, const QPair<K, ComponentModifications>& obj) const
            { addComponentModificationsToResponse(resp, obj.first, obj.second); }
      };

    CMLResponseObjects respObjs;
};

#endif /* UPDATE_COMPONENTS_H_ */
