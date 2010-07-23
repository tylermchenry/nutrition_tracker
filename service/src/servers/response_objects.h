/*
 * response_objects.h
 *
 *  Created on: Jul 23, 2010
 *      Author: tyler
 */

#ifndef RESPONSE_OBJECTS_H_
#define RESPONSE_OBJECTS_H_

template <typename T, typename R>
class ResponseObjects
{
  public:

    ResponseObjects();

    void addObject(const QSharedPointer<const T>& obj);
    void addObjects(const QVector<QSharedPointer<T> >& objs);
    void addObjects(const QVector<QSharedPointer<const T> >& objs);
    void addObjects(const QList<QSharedPointer<const T> >& objs);

    void setError(const QString& errorMessage = "");

    inline bool isEmpty() const { return objects.isEmpty(); }

    QList<QSharedPointer<const T> > getObjects() const;

    virtual R serialize() const;

  protected:

    virtual typename T::cache_key_type getId
      (const QSharedPointer<const T>& obj) const = 0;

    virtual void addObjectToResponse
      (R& resp, const QSharedPointer<const T>& obj) const = 0;

    bool isError() const { return bIsError; }

    QString getErrorMessage() const { return errorMessage; }

    R& addErrors(R& resp) const;

  private:

    bool bIsError;
    QString errorMessage;
    QSet<typename T::cache_key_type> objectIds;
    QList<QSharedPointer<const T> > objects;
};

template <typename T, typename R>
ResponseObjects<T,R>::ResponseObjects()
  : bIsError(false)
{
}

template <typename T, typename R>
void ResponseObjects<T,R>::addObject
  (const QSharedPointer<const T>& obj)
{
  if (obj) {
    typename T::cache_key_type id = getId(obj);
    if (!objectIds.contains(id)) {
      objectIds.insert(id);
      objects.append(obj);
    }
  }
}

template <typename T, typename R>
void ResponseObjects<T,R>::addObjects
  (const QVector<QSharedPointer<T> >& objs)
{
  for (typename QVector<QSharedPointer<T> >::const_iterator i = objs.begin();
       i != objs.end(); ++i)
  {
    addObject(*i);
  }
}

template <typename T, typename R>
void ResponseObjects<T,R>::addObjects
  (const QVector<QSharedPointer<const T> >& objs)
{
  for (typename QVector<QSharedPointer<const T> >::const_iterator i = objs.begin();
       i != objs.end(); ++i)
  {
    addObject(*i);
  }
}

template <typename T, typename R>
void ResponseObjects<T,R>::addObjects
  (const QList<QSharedPointer<const T> >& objs)
{
  for (typename QList<QSharedPointer<const T> >::const_iterator i = objs.begin();
       i != objs.end(); ++i)
  {
    addObject(*i);
  }
}

template <typename T, typename R>
void ResponseObjects<T,R>::setError(const QString& errorMessage)
{
  if (bIsError) {
    this->errorMessage = "Multiple errors occurred while processing this request.";
  } else {
    this->errorMessage = errorMessage;
  }
  bIsError = true;
}

template <typename T, typename R>
QList<QSharedPointer<const T> > ResponseObjects<T,R>::getObjects() const
{
  return objects;
}

template <typename T, typename R>
  R ResponseObjects<T,R>::serialize() const
{
  R resp;

  for (typename QList<QSharedPointer<const T> >::const_iterator i = objects.begin();
       i != objects.end(); ++i)
  {
    addObjectToResponse(resp, *i);
  }

  return addErrors(resp);
}

template <typename T, typename R>
  R& ResponseObjects<T,R>::addErrors(R& resp) const
{
  if (isError()) {
    resp.mutable_error()->set_iserror(true);
    resp.mutable_error()->set_errormessage(getErrorMessage().toStdString());
  }

  return resp;
}


#endif /* RESPONSE_OBJECTS_H_ */
