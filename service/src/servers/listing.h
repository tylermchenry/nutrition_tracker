/*
 * listing.h
 *
 *  Created on: Jul 24, 2010
 *      Author: tyler
 */

#ifndef LISTING_H_
#define LISTING_H_

#include "response_objects.h"
#include <QPair>

template <typename T, typename R, typename K = typename T::cache_key_type>
class Listing : public ResponseObjects<QPair<K, QString>, R, K>
{
  public:

    void addObject(const QSharedPointer<const T>& obj);
    void addObject(const K& id, const QString& name);
    void addObjects(const QMap<K, QString>& objectNames);

  protected:

    virtual K getId
      (const QSharedPointer<const T>& obj) const = 0;

    virtual QString getName
      (const QSharedPointer<const T>& obj) const = 0;

    virtual void addListingToResponse
      (R& resp, const K& id, const QString& name) const = 0;

  private:

    virtual K getId
      (const QSharedPointer<const QPair<K, QString> >& obj) const;

    virtual void addObjectToResponse
      (R& resp, const QSharedPointer<const QPair<K, QString> >& obj) const;
};

template<typename T, typename R, typename K>
void Listing<T,R,K>::addObject(const QSharedPointer<const T>& obj)
{
  if (obj) addObject(getId(obj), getName(obj));
}

template<typename T, typename R, typename K>
void Listing<T,R,K>::addObject(const K& id, const QString& name)
{
  ResponseObjects<QPair<K, QString>, R, K>::addObject
    (QSharedPointer<QPair<K, QString> >(new QPair<K, QString>(id, name)));
}

template<typename T, typename R, typename K>
void Listing<T,R,K>::addObjects(const QMap<K, QString>& objectNames)
{
  for (typename QMap<K, QString>::const_iterator i = objectNames.begin();
      i != objectNames.end(); ++i)
  {
    addObject(i.key(), i.value());
  }
}

template<typename T, typename R, typename K>
K Listing<T,R,K>::getId
  (const QSharedPointer<const QPair<K, QString> >& obj) const
{
  return obj->first;
}


template<typename T, typename R, typename K>
void Listing<T,R,K>::addObjectToResponse
  (R& resp, const QSharedPointer<const QPair<K, QString> >& obj) const
{
  return addListingToResponse(resp, obj->first, obj->second);
}

#endif /* LISTING_H_ */
