#ifndef GROUP_SERVER_H_
#define GROUP_SERVER_H_

#include "libnutrition/proto/service/group_messages.pb.h"
#include "libnutrition/data/group.h"
#include "servers/response_objects.h"
#include <QString>
#include <QSet>

class GroupLoadResponseObjects : public ResponseObjects<Group, GroupLoadResponse>
{
  protected:

    virtual bool isValid
      (const QSharedPointer<const Group>& group) const
        { return !group.isNull(); }

    virtual QString getId
      (const QSharedPointer<const Group>& group) const
        { return group->getId(); }

    virtual void addObjectToResponse
      (GroupLoadResponse& resp, const QSharedPointer<const Group>& group) const
        { *(resp.add_groups()) = group->serialize(); }
};

namespace GroupServer
{
  GroupLoadResponseObjects loadGroups(const GroupLoadRequest& req);
}

#endif /* GROUP_SERVER_H_ */
