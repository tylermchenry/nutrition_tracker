/*
 * user.cpp
 * Part of nutrition_tracker
 *
 *  Created on: Jun 26, 2010
 *      Author: Tyler McHenry <tyler@nerdland.net>
 */

#include "user_impl.h"
#include "libnutrition/data/data_cache.h"
#include "libnutrition/backend/back_end.h"
#include "libnutrition/proto/data/user.pb.h"
#include <QCryptographicHash>

UserImpl::UserImpl(int id, const QString& username, const QString& realName,
            const QString& pwSHA1_hex)
  : id(id), username(username), realName(realName), pwSHA1_hex(pwSHA1_hex)
{
}

UserImpl::~UserImpl()
{
}

void UserImpl::setPassword(const QString& newPassword)
{
  pwSHA1_hex = QCryptographicHash::hash((newPassword + username).toUtf8(),
                                        QCryptographicHash::Sha1).toHex();
}

void UserImpl::saveToDatabase()
{
  BackEnd::getBackEnd()->storeUser(DataCache<User>::getInstance().get(id));
}

UserData UserImpl::serialize() const
{
  UserData udata;

  udata.set_userid(id);
  udata.set_username(username.toStdString());
  udata.set_realname(realName.toStdString());
  udata.set_password_sha1_hex(pwSHA1_hex.toStdString());

  return udata;
}

bool UserImpl::checkPassword(const QString& password) const
{
  return QCryptographicHash::hash((password + username).toUtf8(), QCryptographicHash::Sha1).toHex() ==
    pwSHA1_hex.toUtf8();
}

bool UserImpl::checkPasswordHash(const QString& password_hash) const
{
  return (password_hash == pwSHA1_hex.toUtf8());
}

