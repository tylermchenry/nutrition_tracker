/*
 * modification_listing.h
 *
 *  Created on: Aug 20, 2010
 *      Author: tyler
 */

#ifndef MODIFICATION_LISTING_H_
#define MODIFICATION_LISTING_H_

#include "response_objects.h"
#include <libnutrition/proto/service/storage_modification.pb.h>
#include <QPair>

template <typename T, typename R, typename K = typename T::cache_key_type >
class ModificationListing
{
  public:

    void addModification(const K& oldId, const K& newId)
      { respObjs.addObject(Modification(oldId, newId)); }

    void addModification(const K& oldId, const K& newId,
                            int oldOrder, int newOrder)
      { respObjs.addObject(Modification(oldId, newId, oldOrder, newOrder)); }

    virtual R serialize() const
      { return respObjs.serialize(); }

    virtual R serialize(R& resp) const
      { return respObjs.serialize(resp); }

  private:

    struct Modification {
      K oldId;
      K newId;
      bool orderModified;
      int oldOrder;
      int newOrder;

      Modification() : orderModified(false) {};

      Modification(const K& oid, const K& nid)
        : oldId(oid), newId(nid), orderModified(false)
      {}

      Modification(const K& oid, const K& nid, int oo, int no)
        : oldId(oid), newId(nid), orderModified(true),
          oldOrder(oo), newOrder(no)
      {}
    };

    class MLResponseObjects
      : public ResponseObjects<Modification, R, K, Modification>
    {
      protected:

        virtual bool isValid(const Modification&) const
          { return true; }

        virtual K getId(const Modification& obj) const
          { return obj.newId; }

        virtual K getName(const Modification& obj) const
          { return obj.newId; }

        virtual void addObjectToResponse
          (R& resp, const Modification& obj) const
        {
          StorageModification* smod = resp.add_modifications();

          smod->set_oldid(obj.oldId);
          smod->set_newid(obj.newId);

          if (obj.orderModified) {
            smod->set_oldorder(obj.oldOrder);
            smod->set_neworder(obj.newOrder);
          }
        }
    };

    MLResponseObjects respObjs;
};

#endif /* MODIFICATION_LISTING_H_ */
