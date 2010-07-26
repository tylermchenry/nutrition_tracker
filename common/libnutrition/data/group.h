/*
 * group.h - Data object for food groups (aka categories)
 *
 * This file is part of Nutrition Tracker.
 *
 * Nutrition Tracker is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Nutrition Tracker is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Nutrition Tracker.  If not, see <http://www.gnu.org/licenses/>.
 *
 * Copyright © 2010 Tyler McHenry <tyler@nerdland.net>
 */

#ifndef GROUP_H_
#define GROUP_H_

#include <QVector>
#include <QMap>
#include <QSharedPointer>
#include <QWeakPointer>

class GroupData; // forward decl

/* A food group (or category) is a USDA-defined classification of a single
 * (non-composite) food, such as "Vegetables", "Fast Food", "Milk and Cheese",
 * etc. All single foods have exactly one group. A group is a simple tag; it is
 * composed of an ID and a name.
 */
class Group
{
  public:

    // Definitions to make this class cacheable with DataCache
    typedef QString cache_key_type;
    typedef const Group cache_object_type;
    typedef QString cache_sort_key_type;
    static QString (Group::* const cache_get_sort_key)() const;
    static const bool cache_strong = true;

    static const QString DEFAULT_GROUP_ID;

    static QSharedPointer<const Group> getDefaultGroup();

    static QSharedPointer<const Group> getGroup(const QString& id);

    static QVector<QSharedPointer<const Group> > getAllGroups();

    virtual ~Group() {};

    virtual QString getId() const = 0;

    virtual QString getName() const = 0;

    virtual GroupData serialize() const = 0;

    virtual bool operator== (const Group& rhs) const = 0;

  protected:

    static void setAllLoaded();

  private:

    static QVector<QSharedPointer<const Group> > all;

    friend class GroupImpl;
};

#endif /* GROUP_H_ */
