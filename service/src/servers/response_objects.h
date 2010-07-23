/*
 * response_objects.h
 *
 *  Created on: Jul 23, 2010
 *      Author: tyler
 */

#ifndef RESPONSE_OBJECTS_H_
#define RESPONSE_OBJECTS_H_

template <typename T, typename R, typename K = typename T::cache_key_type>
class ResponseObjects
{
  public:

    ResponseObjects();

    void addObject(const QSharedPointer<const T>& obj);
    void addObjects(const QVector<QSharedPointer<T> >& objs);
    void addObjects(const QVector<QSharedPointer<const T> >& objs);
    void addObjects(const QList<QSharedPointer<const T> >& objs);

    void clear();

    void replaceObjects(const QList<QSharedPointer<const T> >& objs);

    void setError(const QString& errorMessage = "");

    inline bool isEmpty() const { return objects.isEmpty(); }

    bool contains(const QSharedPointer<const T>& obj) const;

    QList<QSharedPointer<const T> > getObjects() const;
    QSet<K> getObjectIds() const;

    virtual R serialize() const;

  protected:

    virtual K getId
      (const QSharedPointer<const T>& obj) const = 0;

    virtual void addObjectToResponse
      (R& resp, const QSharedPointer<const T>& obj) const = 0;

    bool isError() const { return bIsError; }

    QString getErrorMessage() const { return errorMessage; }

    R& addErrors(R& resp) const;

  private:

    bool bIsError;
    QString errorMessage;
    QSet<K> objectIds;
    QList<QSharedPointer<const T> > objects;
};

template <typename T, typename R, typename K>
ResponseObjects<T,R,K>::ResponseObjects()
  : bIsError(false)
{
}

template <typename T, typename R, typename K>
void ResponseObjects<T,R,K>::addObject
  (const QSharedPointer<const T>& obj)
{
  if (obj) {
    K id = getId(obj);
    if (!objectIds.contains(id)) {
      objectIds.insert(id);
      objects.append(obj);
    }
  }
}

template <typename T, typename R, typename K>
void ResponseObjects<T,R,K>::addObjects
  (const QVector<QSharedPointer<T> >& objs)
{
  for (typename QVector<QSharedPointer<T> >::const_iterator i = objs.begin();
       i != objs.end(); ++i)
  {
    addObject(*i);
  }
}

template <typename T, typename R, typename K>
void ResponseObjects<T,R,K>::addObjects
  (const QVector<QSharedPointer<const T> >& objs)
{
  for (typename QVector<QSharedPointer<const T> >::const_iterator i = objs.begin();
       i != objs.end(); ++i)
  {
    addObject(*i);
  }
}

template <typename T, typename R, typename K>
void ResponseObjects<T,R,K>::addObjects
  (const QList<QSharedPointer<const T> >& objs)
{
  for (typename QList<QSharedPointer<const T> >::const_iterator i = objs.begin();
       i != objs.end(); ++i)
  {
    addObject(*i);
  }
}

template <typename T, typename R, typename K>
void ResponseObjects<T,R,K>::clear()
{
  objects.clear();
  objectIds.clear();
}

template <typename T, typename R, typename K>
void  ResponseObjects<T,R,K>::replaceObjects
  (const QList<QSharedPointer<const T> >& objs)
{
  clear();
  addObjects(objs);
}

template <typename T, typename R, typename K>
void ResponseObjects<T,R,K>::setError(const QString& errorMessage)
{
  if (bIsError) {
    this->errorMessage = "Multiple errors occurred while processing this request.";
  } else {
    this->errorMessage = errorMessage;
  }
  bIsError = true;
}

template <typename T, typename R, typename K>
bool ResponseObjects<T,R,K>::contains(const QSharedPointer<const T>& obj) const
{
  return (obj && objectIds.contains(getId(obj)));
}

template <typename T, typename R, typename K>
QList<QSharedPointer<const T> > ResponseObjects<T,R,K>::getObjects() const
{
  return objects;
}

template <typename T, typename R, typename K>
QSet<K> ResponseObjects<T,R,K>::getObjectIds() const
{
  return objectIds;
}

template <typename T, typename R, typename K>
  R ResponseObjects<T,R,K>::serialize() const
{
  R resp;

  for (typename QList<QSharedPointer<const T> >::const_iterator i = objects.begin();
       i != objects.end(); ++i)
  {
    addObjectToResponse(resp, *i);
  }

  return addErrors(resp);
}

template <typename T, typename R, typename K>
  R& ResponseObjects<T,R,K>::addErrors(R& resp) const
{
  if (isError()) {
    resp.mutable_error()->set_iserror(true);
    resp.mutable_error()->set_errormessage(getErrorMessage().toStdString());
  }

  return resp;
}


#endif /* RESPONSE_OBJECTS_H_ */
