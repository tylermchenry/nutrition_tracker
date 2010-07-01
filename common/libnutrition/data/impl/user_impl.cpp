/*
 * user.cpp
 * Part of nutrition_tracker
 *
 *  Created on: Jun 26, 2010
 *      Author: Tyler McHenry <tyler@nerdland.net>
 */

#include "user_impl.h"
#include <QCryptographicHash>
#include <QVariant>
#include <QDebug>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlRecord>
#include <QtSql/QSqlField>
#include <QtSql/QSqlError>

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
  QSqlDatabase db = QSqlDatabase::database("nutrition_db");
  QSqlQuery query(db);

  query.prepare("UPDATE user SET Name=:realName, PW_SHA1=:shapass "
                " WHERE User_Id=:id");
  query.bindValue(":realName", realName);
  query.bindValue(":shapass", pwSHA1_hex);
  query.bindValue(":id", id);

  if (!query.exec()) {
    qDebug() << "SQL Failure: " << query.lastError();
  }
}

bool UserImpl::checkPassword(const QString& password) const
{
  return QCryptographicHash::hash((password + username).toUtf8(), QCryptographicHash::Sha1).toHex() ==
    pwSHA1_hex.toUtf8();
}

