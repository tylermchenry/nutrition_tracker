#include "mysql_back_end.h"
#include "libnutrition/data/data_cache.h"
#include "libnutrition/data/impl/user_impl.h"
#include <QVariant>
#include <QDebug>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlRecord>
#include <QtSql/QSqlField>
#include <QtSql/QSqlError>
#include <stdexcept>

QSharedPointer<User> MySQLBackEnd::loadUser(int id)
{
  QSqlQuery query(db);

  query.prepare("SELECT User_Id, User_Name, Name, PW_SHA1 FROM user "
                 " WHERE User_Id=:id LIMIT 1");
  query.bindValue(":id", id);

  if (query.exec() && query.first()) {
    return createUserFromRecord(query.record());
  } else {
    return QSharedPointer<User>();
  }
}

QSharedPointer<User> MySQLBackEnd::loadUserByUsername
  (const QString& username)
{
  QSqlQuery query(db);

  query.prepare("SELECT User_Id, User_Name, Name, PW_SHA1 FROM user "
                 " WHERE User_Name=:username LIMIT 1");
  query.bindValue(":username", username);

  if (query.exec() && query.first()) {
    return createUserFromRecord(query.record());
  } else {
    return QSharedPointer<User>();
  }
}

QList<QSharedPointer<User> > MySQLBackEnd::loadAllUsers()
{
  QSqlDatabase db = QSqlDatabase::database("nutrition_db");
  QSqlQuery query(db);

  query.prepare("SELECT User_Id, User_Name, Name, PW_SHA1 FROM user "
                 " ORDER BY User_Name ASC");

  if (query.exec()) {
    return createUsersFromQueryResults(query);
  } else {
    return QList<QSharedPointer<User> >();
  }
}

void MySQLBackEnd::storeUser(const QSharedPointer<User>& user)
{
  // TODO: Replace
  user->saveToDatabase();
}

QList<QSharedPointer<User> > MySQLBackEnd::createUsersFromQueryResults
  (QSqlQuery& query)
{
  QList<QSharedPointer<User> > users;

  while (query.next()) {
    users.push_back(createUserFromRecord(query.record()));
  }

  return users;
}

QSharedPointer<User> MySQLBackEnd::createUserFromRecord
  (const QSqlRecord& record)
{
  if (!record.isEmpty()) {
    int id = record.field("User_Id").value().toInt();
    QSharedPointer<User> user
    (new UserImpl(id,
                  record.field("User_Name").value().toString(),
                  record.field("Name").value().toString(),
                  record.field("PW_SHA1").value().toString()));
    DataCache<User>::getInstance().insert(id, user);
    return user;
  } else {
    return QSharedPointer<User>();
  }
}
