#include "servers/user_server.h"
#include "servers/data_server.h"
#include "libnutrition/data/user.h"
#include <cassert>

namespace UserServer {

  UserLoadResponseObjects loadUsers(const UserLoadRequest& req)
  {
    UserLoadResponseObjects resp_objs;

    if (req.omit()) return resp_objs;

    if (req.all()) {

      resp_objs.addObjects(User::getAllUsers());

    } else {

      for (int i = 0; i < req.requestedids_size(); ++i) {
        resp_objs.addObject(User::getUser(req.requestedids(i)));
      }

      for (int i = 0; i < req.requestedusernames_size(); ++i) {
        resp_objs.addObject
          (User::getUserByUsername(QString::fromStdString(req.requestedusernames(i))));
      }

    }

    return resp_objs;
  }

  StoredUserListing storeUsers(const UserStoreRequest& req)
  {
    StoredUserListing confirmations;
    bool accessViolation = false;

    for (int i = 0; i < req.users_size(); ++i) {
      const UserData& userData = req.users(i);

      if (userData.userid() == User::getLoggedInUserId()) {

        QSharedPointer<User> user = User::getUser(userData.userid());

        if (userData.has_realname()) {
          user->setRealName(QString::fromStdString(userData.realname()));
        }

        if (userData.has_password_sha1_hex()) {
          // TODO: Accept password hashes
          if (!userData.has_password_plain()) {
            confirmations.setError("Password for " + user->getUsername() + " "
                                   "was not changed. Hashed passwords are not "
                                   "yet supported when storing users.");
          }
        }

        if (userData.has_password_plain()) {
          user->setPassword(QString::fromStdString(userData.password_plain()));
        }

        if (userData.has_username()) {
          if (user->getUsername() != QString::fromStdString(userData.username())) {
            confirmations.setError("Username for " + user->getUsername() + " "
                                   "was not changed. Changing usernames is not "
                                   "supported.");
          }
        }

        try {
          user->saveToDatabase();
          confirmations.addObject(user);
        } catch (const std::exception& ex) {
          confirmations.setError("Failed to save changes to user " +
                                 user->getUsername() + ". Error was: " +
                                 QString::fromStdString(ex.what()));
        }

      } else {

        accessViolation = true;
      }
    }

    if (accessViolation) {
      confirmations.setError("Some user data was not stored because it does not "
                             "correspond to the currently logged in user.");
    }

    return confirmations;
  }

}
