#include "servers/group_server.h"
#include "servers/data_server.h"
#include "libnutrition/data/group.h"
#include <cassert>

void GroupLoadResponseObjects::addGroup
(const QSharedPointer<const Group>& group)
{
  if (group && !groupIds.contains(group->getId())) {
    groupIds.insert(group->getId());
    groups.append(group);
  }
}

void GroupLoadResponseObjects::addGroups
  (const QVector<QSharedPointer<const Group> >& groups)
{
  for (QVector<QSharedPointer<const Group> >::const_iterator i = groups.begin();
       i != groups.end(); ++i)
  {
    addGroup(*i);
  }
}

void GroupLoadResponseObjects::addGroups
  (const QList<QSharedPointer<const Group> >& groups)
{
  for (QList<QSharedPointer<const Group> >::const_iterator i = groups.begin();
       i != groups.end(); ++i)
  {
    addGroup(*i);
  }
}

QList<QSharedPointer<const Group> > GroupLoadResponseObjects::getGroups() const
{
  return groups;
}

GroupLoadResponse GroupLoadResponseObjects::serialize() const
{
  GroupLoadResponse resp;

  for (QList<QSharedPointer<const Group> >::const_iterator i = groups.begin();
       i != groups.end(); ++i)
  {
    *(resp.add_groups()) = (*i)->serialize();
  }

  return resp;
}

namespace GroupServer {

  GroupLoadResponseObjects loadGroups(const GroupLoadRequest& req)
  {
    GroupLoadResponseObjects resp_objs;

    if (req.omit()) return resp_objs;

    if (req.all()) {

      resp_objs.addGroups(Group::getAllGroups());

    } else {

      for (int i = 0; i < req.requestedids_size(); ++i) {
        resp_objs.addGroup
          (Group::getGroup(QString::fromStdString(req.requestedids(i))));
      }

    }

    return resp_objs;
  }
}
