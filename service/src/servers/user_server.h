#ifndef USER_SERVER_H_
#define USER_SERVER_H_

#include "libnutrition/proto/service/user_messages.pb.h"
#include "libnutrition/data/user.h"
#include "servers/response_objects.h"
#include "servers/listing.h"
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

template <typename R>
class UserListing : public Listing<User, R>
{
  protected:

    virtual bool isValid
      (const QSharedPointer<const User>& user) const
        { return !user.isNull(); }

     virtual int getId
       (const QSharedPointer<const User>& user) const
         { return user->getId(); }

     virtual QString getName
       (const QSharedPointer<const User>& user) const
         { return user->getUsername(); }
};

class StoredUserListing : public UserListing<UserStoreResponse>
{
  protected:

    virtual void addListingToResponse
      (UserStoreResponse& resp, const int& id, const QString&) const
        { resp.add_storedids(id); }
};

namespace UserServer
{
  UserLoadResponseObjects loadUsers(const UserLoadRequest& req);

  StoredUserListing storeUsers(const UserStoreRequest& req);
}

#endif /* USER_SERVER_H_ */
