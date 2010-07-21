#include "mysql_back_end.h"
#include "libnutrition/data/data_cache.h"
#include "libnutrition/data/impl/group_impl.h"
#include <QVariant>
#include <QDebug>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlRecord>
#include <QtSql/QSqlField>
#include <QtSql/QSqlError>
#include <stdexcept>

QSharedPointer<User> MySQLBackEnd::logIn
  (const QString& username, const QString& password, QString& errorMessage)
{
  QSharedPointer<User> user = User::getUserByUsername(username);

  if (!user) {
    errorMessage = "Invalid user object";
  } else if (!user->checkPassword(password)){
    errorMessage = "Incorrect password";
  } else {
    return user;
  }

  return QSharedPointer<User>();
}

QSharedPointer<User> MySQLBackEnd::logInWithPasswordHash
  (const QString& username, const QString& password_hash, QString& errorMessage)
{
  QSharedPointer<User> user = User::getUserByUsername(username);

  if (!user) {
    errorMessage = "Invalid user object";
  } else if (!user->checkPasswordHash(password_hash)){
    errorMessage = "Incorrect password";
  } else {
    return user;
  }

  return QSharedPointer<User>();
}

QSharedPointer<User>
  MySQLBackEnd::registerAndLogInWithPasswordHash
    (const QString& username, const QString& realName,
     const QString& password_hash, QString& errorMessage)
{
  if (User::getUserByUsername(username)) {
    errorMessage = "Username is already in use";
    return QSharedPointer<User>();
  }

  QSqlQuery query(db);

  query.prepare("INSERT INTO user (User_Id, User_Name, Name, PW_SHA1) "
                " VALUES (NULL, :username, :realName, :passwordhash)");
  query.bindValue(":username", username);
  query.bindValue(":realName", realName);
  query.bindValue(":passwordhash", password_hash);

  if (query.exec()) {
    QSharedPointer<User> user = User::getUser(query.lastInsertId().toInt());
    if (!user) {
      errorMessage = "Unknown failure";
    }
    return user;
  } else {
    errorMessage = "SQL Failure: " + query.lastError().text();
    return QSharedPointer<User>();
  }
}

QSharedPointer<User>
  MySQLBackEnd::registerAndLogIn
    (const QString& username, const QString& realName,
     const QString& password, QString& errorMessage)
{
  if (User::getUserByUsername(username)) {
    errorMessage = "Username is already in use";
    return QSharedPointer<User>();
  } else if (password.isEmpty()) {
    errorMessage = "Password cannot be blank";
    return QSharedPointer<User>();
  }

  QSqlQuery query(db);

  query.prepare("INSERT INTO user (User_Id, User_Name, Name, PW_SHA1) "
                " VALUES (NULL, :username, :realName, "
                "         SHA1(CONCAT(:password, :username2)))");
  query.bindValue(":username", username);
  query.bindValue(":username2", username);
  query.bindValue(":realName", realName);
  query.bindValue(":password", password);

  if (query.exec()) {
    QSharedPointer<User> user = User::getUser(query.lastInsertId().toInt());
    if (!user) {
      errorMessage = "Unknown failure";
    }
    return user;
  } else {
    errorMessage = "SQL Failure: " + query.lastError().text();
    return QSharedPointer<User>();
  }
}
