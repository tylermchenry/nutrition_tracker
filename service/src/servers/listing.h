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
#include <QMultiMap>

template <typename T, typename R, typename K = typename T::cache_key_type,
           typename V = QSharedPointer<const T> >
class Listing : public ResponseObjects<QPair<K, QString>, R, K, QPair<K, QString> >
{
  public:

    void addObject(const V& obj);
    void addObject(const K& id, const QString& name);
    void addObjects(const QList<V>& objs);
    void addObjects(const QMap<K, QString>& objectNames);
    void addObjects(const QMultiMap<QString, K>& objectNames);

  protected:

    virtual bool isValid (const V& obj) const = 0;

    virtual K getId (const V& obj) const = 0;

    virtual QString getName (const V& obj) const = 0;

    virtual void addListingToResponse
      (R& resp, const K& id, const QString& name) const = 0;

  private:

    virtual bool isValid (const QPair<K, QString>& obj) const;

    virtual K getId (const QPair<K, QString>& obj) const;

    virtual void addObjectToResponse
      (R& resp, const QPair<K, QString>& obj) const;
};

template<typename T, typename R, typename K, typename V>
void Listing<T,R,K,V>::addObject(const V& obj)
{
  if (isValid(obj)) addObject(getId(obj), getName(obj));
}

template<typename T, typename R, typename K, typename V>
void Listing<T,R,K,V>::addObject(const K& id, const QString& name)
{
  ResponseObjects<QPair<K, QString>, R, K, QPair<K, QString> >::
    addObject(qMakePair(id, name));
}

template<typename T, typename R, typename K, typename V>
void Listing<T,R,K,V>::addObjects(const QList<V>& objs)
{
  for (typename QList<V>::const_iterator i = objs.begin();
      i != objs.end(); ++i)
  {
    addObject(*i);
  }
}

template<typename T, typename R, typename K, typename V>
void Listing<T,R,K,V>::addObjects(const QMap<K, QString>& objectNames)
{
  for (typename QMap<K, QString>::const_iterator i = objectNames.begin();
      i != objectNames.end(); ++i)
  {
    addObject(i.key(), i.value());
  }
}

template<typename T, typename R, typename K, typename V>
void Listing<T,R,K,V>::addObjects(const QMultiMap<QString, K>& objectNames)
{
  for (typename QMultiMap<QString, K>::const_iterator i = objectNames.begin();
      i != objectNames.end(); ++i)
  {
    addObject(i.value(), i.key());
  }
}

template<typename T, typename R, typename K, typename V>
bool Listing<T,R,K,V>::isValid
  (const QPair<K, QString>&) const
{
  return true;
}

template<typename T, typename R, typename K, typename V>
K Listing<T,R,K,V>::getId
  (const QPair<K, QString>& obj) const
{
  return obj.first;
}

template<typename T, typename R, typename K, typename V>
void Listing<T,R,K,V>::addObjectToResponse
  (R& resp, const QPair<K, QString>& obj) const
{
  return addListingToResponse(resp, obj.first, obj.second);
}

#endif /* LISTING_H_ */
