/*
 * user.cpp
 * Part of nutrition_tracker
 *
 *  Created on: Jun 26, 2010
 *      Author: Tyler McHenry <tyler@nerdland.net>
 */

#include "user.h"
#include <QCryptographicHash>
#include <QVariant>
#include <QDebug>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlRecord>
#include <QtSql/QSqlField>
#include <QtSql/QSqlError>

QMap<int, QWeakPointer<User> > User::userCache;
QSharedPointer<User> User::loggedInUser;

QSharedPointer<const User> User::getUser(int id)
{
  QSqlDatabase db = QSqlDatabase::database("nutrition_db");
  QSqlQuery query(db);

  if (userCache[id]) {
    return userCache[id].toStrongRef();
  }

  query.prepare("SELECT User_Id, User_Name, Name, PW_SHA1 FROM user "
                 " WHERE User_Id=:id LIMIT 1");
  query.bindValue(":id", id);

  if (query.exec() && query.first()) {
    return createUserFromRecord(query.record());
  } else {
    return QSharedPointer<const User>();
  }
}

QSharedPointer<const User> User::getUserByUsername(const QString& username)
{
  QSqlDatabase db = QSqlDatabase::database("nutrition_db");
  QSqlQuery query(db);

  query.prepare("SELECT User_Id, User_Name, Name, PW_SHA1 FROM user "
                 " WHERE User_Name=:username LIMIT 1");
  query.bindValue(":username", username);

  if (query.exec() && query.first()) {
    return createUserFromRecord(query.record());
  } else {
    return QSharedPointer<const User>();
  }
}

QVector<QSharedPointer<const User> > User::getAllUsers()
{
  QSqlDatabase db = QSqlDatabase::database("nutrition_db");
  QSqlQuery query(db);

  query.prepare("SELECT User_Id, User_Name, Name, PW_SHA1 FROM user "
                 " ORDER BY User_Name ASC");

  if (query.exec()) {
    return createUsersFromQueryResults(query);
  } else {
    return QVector<QSharedPointer<const User> >();
  }
}

QSharedPointer<const User> User::createUser
  (const QString& username, const QString& password,
   const QString& realName, QString& errorMessage)
{
  if (getUserByUsername(username)) {
    errorMessage = "Username is already in use";
    return QSharedPointer<const User>();
  } else if (password.isEmpty()) {
    errorMessage = "Password cannot be blank";
    return QSharedPointer<const User>();
  }

  QSqlDatabase db = QSqlDatabase::database("nutrition_db");
  QSqlQuery query(db);

  query.prepare("INSERT INTO user (User_Id, User_Name, Name, PW_SHA1) "
                " VALUES (NULL, :username, :realName, "
                "         SHA1(CONCAT(:password, :username2)))");
  query.bindValue(":username", username);
  query.bindValue(":username2", username);
  query.bindValue(":realName", realName);
  query.bindValue(":password", password);

  if (query.exec()) {
    QSharedPointer<const User> user = getUser(query.lastInsertId().toInt());
    if (!user) {
      errorMessage = "Unknown failure";
    }
    return user;
  } else {
    errorMessage = "SQL Failure: " + query.lastError().text();
    return QSharedPointer<const User>();
  }

}

bool User::logInAs
  (const QSharedPointer<const User>& user, const QString& password,
   QString& errorMessage)
{
  if (!user) {
    errorMessage = "Invalid user object";
  } else if (!user->checkPassword(password)){
    errorMessage = "Incorrect password";
  } else {
    loggedInUser = userCache[user->getId()].toStrongRef();
    return true;
  }

  return false;
}
QSharedPointer<const User> User::createUserFromRecord
  (const QSqlRecord& record)
{
  if (!record.isEmpty()) {
    int id = record.field("User_Id").value().toInt();
    if (!userCache[id]) {
      QSharedPointer<User> user
      (new User(id,
                record.field("User_Name").value().toString(),
                record.field("Name").value().toString(),
                record.field("PW_SHA1").value().toString()));
      userCache[id] = user;
      return user;
    } else {
      return userCache[id].toStrongRef();
    }
  } else {
    return QSharedPointer<const User>();
  }
}

QVector<QSharedPointer<const User> > User::createUsersFromQueryResults
  (QSqlQuery& query)
{
  QVector<QSharedPointer<const User> > users;

  while (query.next()) {
    users.push_back(createUserFromRecord(query.record()));
  }

  return users;
}

User::User(int id, const QString& username, const QString& realName,
            const QString& pwSHA1_hex)
  : id(id), username(username), realName(realName), pwSHA1_hex(pwSHA1_hex)
{
}

User::~User()
{
}

void User::setPassword(const QString& newPassword)
{
  pwSHA1_hex = QCryptographicHash::hash((newPassword + username).toUtf8(),
                                        QCryptographicHash::Sha1).toHex();
}

void User::saveToDatabase()
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

bool User::checkPassword(const QString& password) const
{
  return QCryptographicHash::hash((password + username).toUtf8(), QCryptographicHash::Sha1).toHex() ==
    pwSHA1_hex.toUtf8();
}

