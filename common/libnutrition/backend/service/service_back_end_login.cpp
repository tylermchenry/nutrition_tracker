/*
 * service_back_end_login.cpp
 * Part of libnutrition
 *
 *  Created on: Jul 4, 2010
 *      Author: Tyler McHenry <tyler@nerdland.net>
 */

#include "service_back_end.h"
#include "libnutrition/proto/service/login_messages.pb.h"
#include "libnutrition/data/impl/user_impl.h"
#include "libnutrition/data/data_cache.h"
#include <QCryptographicHash>

QSharedPointer<User> ServiceBackEnd::logIn
  (const QString& username, const QString& password, QString& errorMessage)
{
  return logInWithPasswordHash
    (username,
     QCryptographicHash::hash((password + username).toUtf8(), QCryptographicHash::Sha1).toHex(),
     errorMessage);
}

QSharedPointer<User> ServiceBackEnd::logInWithPasswordHash
  (const QString& username, const QString& password_hash, QString& errorMessage)
{
  LogInRequest req;
  LogInResponse resp;

  req.set_username(username.toStdString());
  req.set_password_sha1_hex(password_hash.toStdString());

  writeMessageAndReadResponse(req, resp);

  if (resp.has_error() && resp.error().iserror()) {
    errorMessage = QString::fromStdString(resp.error().errormessage());
    return QSharedPointer<User>();
  }

  QSharedPointer<User> user
    (new UserImpl
       (resp.userid(),
        QString::fromStdString(resp.username()),
        QString::fromStdString(resp.realname()),
        ""));

  DataCache<User>::getInstance().insert(user->getId(), user);

  return user;
}

QSharedPointer<User> ServiceBackEnd::registerAndLogInWithPasswordHash
  (const QString& username, const QString& realName,
   const QString& password_hash, QString& errorMessage)
{
  LogInRequest req;
  LogInResponse resp;

  req.set_username(username.toStdString());
  req.set_password_sha1_hex(password_hash.toStdString());
  req.set_realname(realName.toStdString());
  req.set_createuser(true);

  writeMessageAndReadResponse(req, resp);

  if (resp.has_error() && resp.error().iserror()) {
    errorMessage = QString::fromStdString(resp.error().errormessage());
    return QSharedPointer<User>();
  }

  QSharedPointer<User> user
    (new UserImpl
       (resp.userid(),
        QString::fromStdString(resp.username()),
        QString::fromStdString(resp.realname()),
        ""));

  DataCache<User>::getInstance().insert(user->getId(), user);

  return user;
}

QSharedPointer<User> ServiceBackEnd::registerAndLogIn
  (const QString& username, const QString& realName,
   const QString& password, QString& errorMessage)
{
  return registerAndLogInWithPasswordHash
    (username, realName,
     QCryptographicHash::hash((password + username).toUtf8(), QCryptographicHash::Sha1).toHex(),
     errorMessage);
}
