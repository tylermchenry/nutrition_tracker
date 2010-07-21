#ifndef GROUP_SERVER_H_
#define GROUP_SERVER_H_

#include "libnutrition/proto/service/group_messages.pb.h"
#include "libnutrition/data/group.h"
#include <QString>
#include <QSet>

class GroupLoadResponseObjects
{
  public:

    void addGroup(const QSharedPointer<const Group>& group);
    void addGroups(const QVector<QSharedPointer<const Group> >& groups);
    void addGroups(const QList<QSharedPointer<const Group> >& groups);

    inline bool isEmpty() const { return groups.isEmpty(); }

    QList<QSharedPointer<const Group> > getGroups() const;

    GroupLoadResponse serialize() const;

  private:

    QSet<QString> groupIds;
    QList<QSharedPointer<const Group> > groups;
};

namespace GroupServer
{
  GroupLoadResponseObjects loadGroups(const GroupLoadRequest& req);
}

#endif /* GROUP_SERVER_H_ */
