/*
 * response_objects.h
 *
 *  Created on: Jul 23, 2010
 *      Author: tyler
 */

#ifndef RESPONSE_OBJECTS_H_
#define RESPONSE_OBJECTS_H_

#include <QSharedPointer>

template <typename T, typename R, typename K = typename T::cache_key_type,
           typename V = QSharedPointer<const T> >
class ResponseObjects
{
  public:

    ResponseObjects();
    virtual ~ResponseObjects() {}

    void addObject(const V& obj);
    void addObjects(const QVector<V>& objs);
    void addObjects(const QVector<QSharedPointer<T> >& objs);
    void addObjects(const QList<V>& objs);

    void removeObjects(const QSet<K>& ids);
    void clear();

    void replaceObjects(const QList<V>& objs);

    void setError(const QString& errorMessage = "");

    inline bool isEmpty() const { return objects.isEmpty(); }

    bool contains(const V& obj) const;

    QList<V> getObjects() const;
    QSet<K> getObjectIds() const;

    virtual R serialize() const;
    virtual R serialize(R& resp) const;

  protected:

    virtual bool isValid (const V& obj) const = 0;

    virtual K getId (const V& obj) const = 0;

    virtual void addObjectToResponse (R& resp, const V& obj) const = 0;

    bool isError() const { return bIsError; }

    QString getErrorMessage() const { return errorMessage; }

    R& addErrors(R& resp) const;

  private:

    bool bIsError;
    QString errorMessage;
    QSet<K> objectIds;
    QList<V> objects;
};

template <typename T, typename R, typename K, typename V>
ResponseObjects<T,R,K,V>::ResponseObjects()
  : bIsError(false)
{
}

template <typename T, typename R, typename K, typename V>
void ResponseObjects<T,R,K,V>::addObject
  (const V& obj)
{
  if (isValid(obj)) {
    K id = getId(obj);
    if (!objectIds.contains(id)) {
      objectIds.insert(id);
      objects.append(obj);
    }
  }
}

template <typename T, typename R, typename K, typename V>
void ResponseObjects<T,R,K,V>::addObjects
  (const QVector<V>& objs)
{
  for (typename QVector<V>::const_iterator i = objs.begin();
       i != objs.end(); ++i)
  {
    addObject(*i);
  }
}

template <typename T, typename R, typename K, typename V>
void ResponseObjects<T,R,K,V>::addObjects
  (const QVector<QSharedPointer<T> >& objs)
{
  for (typename QVector<QSharedPointer<T> >::const_iterator i = objs.begin();
       i != objs.end(); ++i)
  {
    addObject(*i);
  }
}

template <typename T, typename R, typename K, typename V>
void ResponseObjects<T,R,K,V>::addObjects
  (const QList<V>& objs)
{
  for (typename QList<V>::const_iterator i = objs.begin();
       i != objs.end(); ++i)
  {
    addObject(*i);
  }
}

template <typename T, typename R, typename K, typename V>
void ResponseObjects<T,R,K,V>::removeObjects(const QSet<K>& ids)
{
  objectIds -= ids;
  for (typename QSet<K>::const_iterator i = ids.begin(); i != ids.end(); ++i)
  {
    objects.remove(*i);
  }
}

template <typename T, typename R, typename K, typename V>
void ResponseObjects<T,R,K,V>::clear()
{
  objects.clear();
  objectIds.clear();
}

template <typename T, typename R, typename K, typename V>
void  ResponseObjects<T,R,K,V>::replaceObjects
  (const QList<V>& objs)
{
  clear();
  addObjects(objs);
}

template <typename T, typename R, typename K, typename V>
void ResponseObjects<T,R,K,V>::setError(const QString& errorMessage)
{
  if (bIsError) {
    this->errorMessage = "Multiple errors occurred while processing this request.";
  } else {
    this->errorMessage = errorMessage;
  }
  bIsError = true;
}

template <typename T, typename R, typename K, typename V>
bool ResponseObjects<T,R,K,V>::contains(const V& obj) const
{
  return (obj && objectIds.contains(getId(obj)));
}

template <typename T, typename R, typename K, typename V>
QList<V> ResponseObjects<T,R,K,V>::getObjects() const
{
  return objects;
}

template <typename T, typename R, typename K, typename V>
QSet<K> ResponseObjects<T,R,K,V>::getObjectIds() const
{
  return objectIds;
}

template <typename T, typename R, typename K, typename V>
  R ResponseObjects<T,R,K,V>::serialize() const
{
  R resp;
  return serialize(resp);
}

template <typename T, typename R, typename K, typename V>
  R ResponseObjects<T,R,K,V>::serialize(R& resp) const
{
  for (typename QList<V>::const_iterator i = objects.begin();
       i != objects.end(); ++i)
  {
    addObjectToResponse(resp, *i);
  }

  return addErrors(resp);
}

template <typename T, typename R, typename K, typename V>
  R& ResponseObjects<T,R,K,V>::addErrors(R& resp) const
{
  if (isError()) {
    if (resp.has_error() && resp.error().iserror()) {
      resp.mutable_error()->set_errormessage
        ("Multiple errors occurred while processing this request.");
    } else {
      resp.mutable_error()->set_errormessage(getErrorMessage().toStdString());
    }
    resp.mutable_error()->set_iserror(true);
  }

  return resp;
}


#endif /* RESPONSE_OBJECTS_H_ */
