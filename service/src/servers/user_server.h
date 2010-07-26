#ifndef USER_SERVER_H_
#define USER_SERVER_H_

#include "libnutrition/proto/service/user_messages.pb.h"
#include "libnutrition/data/user.h"
#include "servers/response_objects.h"
#include <QString>
#include <QSet>

class UserLoadResponseObjects : public ResponseObjects<User, UserLoadResponse>
{
  protected:

    virtual bool isValid
      (const QSharedPointer<const User>& user) const
        { return !user.isNull(); }

    virtual int getId
      (const QSharedPointer<const User>& user) const
        { return user->getId(); }

    virtual void addObjectToResponse
      (UserLoadResponse& resp, const QSharedPointer<const User>& user) const
        { *(resp.add_users()) = user->serialize(true); }
};

namespace UserServer
{
  UserLoadResponseObjects loadUsers(const UserLoadRequest& req);
}

#endif /* USER_SERVER_H_ */
