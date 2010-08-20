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

  ComponentModifications updateComponentModifications
    (const QSharedPointer<FoodCollection>& collection,
     const ComponentModifications& cmods);
}

/**
 * CK is the type of Collection Key, which is they type of the collectionId
 * field within the componentModifications sub-message of the response type R.
 * If this is a fundamental type (e.g. int), CK should be that type. If it
 * is structured sub-message, CK should be a *pointer* to the type of the sub-
 * message.
 */
template <typename K, typename R, typename CK = K>
class ComponentModificationListing
{
  public:

    typedef UpdateComponents::ComponentModifications ComponentModifications;

    ComponentModificationListing();

    void addModifications(const K& id, const ComponentModifications& mods);

    virtual R serialize() const;
    virtual R serialize(R& resp) const;

  protected:

    virtual void setId(CK& pb_id, const K& id) const = 0;

  private:

    class CMLResponseObjects
      : public ResponseObjects<QPair<K, ComponentModifications>,
                                R, K, QPair<K, ComponentModifications> >
      {
        public:

          CMLResponseObjects(const ComponentModificationListing& listing);

        protected:

          virtual bool isValid
            (const QPair<K, ComponentModifications>&) const;

          virtual K getId
            (const QPair<K, ComponentModifications>& obj) const;

          virtual ComponentModifications getName
            (const QPair<K, ComponentModifications>& obj) const;

          virtual void addObjectToResponse
            (R& resp, const QPair<K, ComponentModifications>& obj) const;

        private:

          const ComponentModificationListing& listing;

          template <typename T>
            void addModifications
              (T* pb_mods, const K& id, const ComponentModifications& mods) const;

          template <typename S, typename T>
            void setId(S*, T* pb_mods, const K& id) const;

          template <typename S, typename T>
            void setId(S, T* pb_mods, const K& id) const;
      };

    CMLResponseObjects respObjs;
};

template <typename K, typename R, typename CK>
ComponentModificationListing<K,R,CK>::ComponentModificationListing()
  : respObjs(*this)
{
}

template <typename K, typename R, typename CK>
void ComponentModificationListing<K,R,CK>::addModifications
  (const K& id, const ComponentModifications& mods)
{
  if (mods.isError()) {
    respObjs.setError(mods.getErrorMessage());
  }

  return respObjs.addObject(qMakePair(id, mods));
}

template <typename K, typename R, typename CK>
R ComponentModificationListing<K,R,CK>::serialize() const
{
  return respObjs.serialize();
}

template <typename K, typename R, typename CK>
R ComponentModificationListing<K,R,CK>::serialize(R& resp) const
{
  return respObjs.serialize(resp);
}

template <typename K, typename R, typename CK>
ComponentModificationListing<K,R,CK>::CMLResponseObjects::
  CMLResponseObjects(const ComponentModificationListing& listing)
    : listing(listing)
{
}

template <typename K, typename R, typename CK>
bool ComponentModificationListing<K,R,CK>::CMLResponseObjects::isValid
  (const QPair<K, ComponentModifications>&) const
{
  return true;
}

template <typename K, typename R, typename CK>
K ComponentModificationListing<K,R,CK>::CMLResponseObjects::getId
  (const QPair<K, ComponentModifications>& obj) const
{
  return obj.first;
}

template <typename K, typename R, typename CK>
typename ComponentModificationListing<K,R,CK>::ComponentModifications
ComponentModificationListing<K,R,CK>::CMLResponseObjects::getName
  (const QPair<K, ComponentModifications>& obj) const
{
  return obj.second;
}

template <typename K, typename R, typename CK>
void ComponentModificationListing<K,R,CK>::CMLResponseObjects::
  addObjectToResponse
    (R& resp, const QPair<K, ComponentModifications>& obj) const
{
   addModifications(resp.add_componentmodifications(), obj.first, obj.second);
}

/**
 * The following three methods, along with the setId method in the main listing
 * object, accomplish a crude form of duck-typing. The issue is that the type
 * of the pb_mods sub-message is distinct between response messages for stores
 * of different collection-derived types. However, they all have the same
 * physical structure: one field named collectionid, and another repeated field
 * named componentmodifications.
 *
 * The latter field is always of the type ComponentModification, so this is
 * easy to handle. The hard part is the collectionid, which is a simple int
 * for composite foods and templates, but a structure (sub-message) for meals.
 * Thus, we need a virtual function callback to set this id (the pure-virtual
 * setId method in the listing class). But moreover, we need to be able to
 * distinguish between structure ids and integer ids at compile time, because
 * the syntax and methods names for setting these two types of data in a
 * protocol buffer are completely different!
 *
 * What helps us distinguish is the CK template prameter (for collection key),
 * which is the protobuffer type that represents the key. If it is a structured
 * key, it should be a pointer, otherwise it should not be. The
 * CMLResponseObjects class has an overloaded setId method which takes either a
 * pointer or not-a-pointer and uses the integral or structured means of setting
 * the ID depending on which is instantiated.
 */

template <typename K, typename R, typename CK>
template <typename T>
  void ComponentModificationListing<K,R,CK>::CMLResponseObjects::
    addModifications(T* pb_mods, const K& id, const ComponentModifications& mods) const
{
  QSet<int> modifiedIds = mods.idAssignments.keys().toSet();
  modifiedIds.unite(mods.orderChanges.keys().toSet());

  setId(CK(), pb_mods, id);

  for (QSet<int>::const_iterator i = modifiedIds.begin();
       i != modifiedIds.end(); ++i)
  {
    ::ComponentModification* cmod = pb_mods->add_componentmodifications();

    if (mods.idAssignments.contains(*i)) {
      cmod->set_oldcomponentid(mods.idAssignments[*i]);
    } else {
      cmod->set_oldcomponentid(*i); // Set anyway for easy client lookup
    }

    cmod->set_newcomponentid(*i); // Always set this for consistency

    if (mods.orderChanges.contains(*i)) {
      cmod->set_oldorder(mods.orderChanges[*i].first);
      cmod->set_neworder(mods.orderChanges[*i].second);
    }
  }
}

template <typename K, typename R, typename CK>
template <typename S, typename T>
  void ComponentModificationListing<K,R,CK>::CMLResponseObjects::
  setId(S*, T* pb_mods, const K& id) const
{
  CK pb_id = pb_mods->mutable_collectionid();
  listing.setId(pb_id, id);
}


template <typename K, typename R, typename CK>
template <typename S, typename T>
  void ComponentModificationListing<K,R,CK>::CMLResponseObjects::
  setId(S, T* pb_mods, const K& id) const
{
  CK pb_id;
  listing.setId(pb_id, id);
  pb_mods->set_collectionid(pb_id);
}

#endif /* UPDATE_COMPONENTS_H_ */
