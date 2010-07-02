/*
 * user.cpp
 * Part of nutrition_tracker
 *
 *  Created on: Jun 26, 2010
 *      Author: Tyler McHenry <tyler@nerdland.net>
 */

#include "user.h"
#include "impl/user_impl.h"
#include "data_cache.h"
#include "libnutrition/backend/back_end.h"

QString (User::* const User::cache_get_sort_key)() const = &User::getDisplayName;

QSharedPointer<User> User::loggedInUser;

QSharedPointer<User> User::getUser(int id)
{
  if (DataCache<User>::getInstance().contains(id)) {
    return DataCache<User>::getInstance().get(id);
  } else {
    return BackEnd::getBackEnd()->loadUser(id);
  }
}

QSharedPointer<User> User::getUserByUsername(const QString& username)
{
  return BackEnd::getBackEnd()->loadUserByUsername(username);
}

QVector<QSharedPointer<User> > User::getAllUsers()
{
  // TODO: Change return value to list
  return BackEnd::getBackEnd()->loadAllUsers().toVector();
}

QSharedPointer<User> User::createUser
  (const QString& username, const QString& password,
   const QString& realName, QString& errorMessage)
{
  QSharedPointer<User> user =
    BackEnd::getBackEnd()->registerAndLogIn
      (username, realName, password, errorMessage);

  if (user) {
    loggedInUser = user;
  }

  return user;
}

QSharedPointer<User> User::logInAs
  (const QString& username, const QString& password, QString& errorMessage)
{
  QSharedPointer<User> user = BackEnd::getBackEnd()->logIn
    (username, password, errorMessage);

  if (user) {
    loggedInUser = user;
  }

  return user;
}

User::~User()
{
}
