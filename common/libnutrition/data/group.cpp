/*
 * group.cpp
 * Part of nutrition_tracker
 *
 *  Created on: May 16, 2010
 *      Author: Tyler McHenry <tyler@nerdland.net>
 */

#include "group.h"
#include "impl/group_impl.h"
#include "data_cache.h"
#include "libnutrition/backend/back_end.h"

const QString Group::DEFAULT_GROUP_ID = "9999";

QString (Group::* const Group::cache_get_sort_key)() const = &Group::getName;

QSharedPointer<const Group> Group::getDefaultGroup()
{
  return getGroup(DEFAULT_GROUP_ID);
}

QSharedPointer<const Group> Group::getGroup(const QString& id)
{
  if (DataCache<Group>::getInstance().contains(id)) {
    return DataCache<Group>::getInstance().get(id);
  } else {
    return BackEnd::getBackEnd()->loadGroup(id);
  }
}

QVector<QSharedPointer<const Group> > Group::getAllGroups()
{
  static bool gotAll = false;

  if (gotAll) {
    static QVector<QSharedPointer<const Group> > groupsByName;

    if (groupsByName.empty()) {
      groupsByName = DataCache<Group>::getInstance().getAll().toVector();
    }

    return groupsByName;
  }

  QList<QSharedPointer<Group> > groups = BackEnd::getBackEnd()->loadAllGroups();

  gotAll = !groups.empty();

  QVector<QSharedPointer<const Group> > groupsVec;

  for (QList<QSharedPointer<Group> >::const_iterator i = groups.begin();
       i != groups.end(); ++i)
  {
    groupsVec.push_back(*i);
  }

  return groupsVec;
}
