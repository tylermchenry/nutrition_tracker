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

QVector<QSharedPointer<const Group> > Group::all;

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
  if (all.isEmpty()) {
    BackEnd::getBackEnd()->loadAllGroups();
    setAllLoaded();
  }

  return all;
}

void Group::setAllLoaded()
{
  all = DataCache<Group>::getInstance().getAll().toVector();
}
