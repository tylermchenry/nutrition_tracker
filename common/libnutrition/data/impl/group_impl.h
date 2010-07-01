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

#ifndef GROUP_IMPL_H_
#define GROUP_IMPL_H_

#include "libnutrition/data/group.h"
#include <QString>
#include <QVector>
#include <QMap>
#include <QSharedPointer>

/* A food group (or category) is a USDA-defined classification of a single
 * (non-composite) food, such as "Vegetables", "Fast Food", "Milk and Cheese",
 * etc. All single foods have exactly one group. A group is a simple tag; it is
 * composed of an ID and a name.
 */
class GroupImpl : virtual public Group
{
  public:

    GroupImpl(const QString& id, const QString& name);

    virtual ~GroupImpl();

    virtual inline QString getId() const { return id; }

    virtual inline QString getName() const { return name; }

    virtual inline bool operator== (const Group& rhs) const
      { return (id == rhs.getId()); }

  private:

    QString id;
    QString name;
};

#endif /* GROUP_IMPL_H_ */
