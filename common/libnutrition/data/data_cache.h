/*
 * data_cache.h
 * Part of libnutrition
 *
 *  Created on: Jul 1, 2010
 *      Author: Tyler McHenry <tyler@nerdland.net>
 */

#ifndef DATA_CACHE_H_
#define DATA_CACHE_H_

#include <QMap>
#include <QMultiMap>
#include <QSharedPointer>
#include <QWeakPointer>

template<typename T> class DataCache
{
  private:

    // Types wanting to be cached must define a static boolean cache_strong.
    // If this value is true, then shared pointers are stored (so the object
    // is never deleted until it is explicitly removed from the cache).
    // Otherwise, weak pointers are stored, and the object is deleted when the
    // last non-cache strong reference is destroyed.

    template<typename S, bool Strong> struct internal_pointer;

    template<typename S> struct internal_pointer<S, true>
    {
        typedef QSharedPointer<S> type;
    };

    template<typename S> struct internal_pointer<S, false>
    {
        typedef QWeakPointer<S> type;
    };


  public:

    typedef typename T::cache_key_type key_type;
    typedef typename T::cache_object_type object_type;
    typedef typename internal_pointer<object_type, T::cache_strong>::type
      internal_pointer_type;
    typedef QSharedPointer<object_type> external_pointer_type;

    // Not typedefing cache_sort_key_type here because we want classes to be
    // able to omit that typedef (and cache_get_sort_key) if they never intend
    // to be used with getAll()

    static DataCache<T>& getInstance()
    {
      static DataCache<T> dataCache;
      return dataCache;
    }

    void insert(const key_type& key, const external_pointer_type& val)
    {
      cache.insert(key, val);
    }

    bool contains(const key_type& key) const
    {
      return cache.contains(key) && (T::cache_strong || cache[key]);
    }

    external_pointer_type get(const key_type& key) const
    {
      if (contains(key)) {
        return cache[key];
      } else {
        return external_pointer_type();
      }
    }

    QList<external_pointer_type> getAll() const
    {
      typedef typename T::cache_sort_key_type sort_key_type;

      QList<internal_pointer_type> all = cache.values();
      QMultiMap<sort_key_type, external_pointer_type> sortedAll;

      for (typename QList<internal_pointer_type>::const_iterator i = all.begin();
           i != all.end(); ++i)
      {
        if (*i) {
          // This here is some ridiculous fucking syntax I tell you what.
          // Somehow, magically, it works with **i and .* but not with *i and ->*
          sortedAll.insert(((**i).*(T::cache_get_sort_key))(), *i);
        }
      }

      return sortedAll.values();
    }

    void remove(const key_type& key)
    {
      cache.remove(key);
    }

    void changeKey(const key_type& oldKey, const key_type& newKey)
    {
      if (oldKey != newKey) {
        cache[newKey] = cache[oldKey];
        remove(oldKey);
      }
    }

    void clear()
    {
      cache.clear();
    }

  private:

    DataCache() {};

    QMap<key_type, internal_pointer_type> cache;
};


#endif /* DATA_CACHE_H_ */
