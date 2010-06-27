/*
 * user.h
 * Part of nutrition_tracker
 *
 *  Created on: Jun 26, 2010
 *      Author: Tyler McHenry <tyler@nerdland.net>
 */

#ifndef USER_H_
#define USER_H_

#include <QString>
#include <QVector>
#include <QSharedPointer>
#include <QWeakPointer>
#include <QtSql/QSqlQuery>

class User
{
  public:

    static QSharedPointer<const User> getUser(int id);

    static QSharedPointer<const User> getUserByUsername
      (const QString& username);

    static QVector<QSharedPointer<const User> > getAllUsers();

    static QSharedPointer<const User> createUser
      (const QString& username, const QString& password,
       const QString& realName, QString& errorMessage);

    static bool logInAs
      (const QSharedPointer<const User>& user, const QString& password,
       QString& errorMessage);

    inline static QSharedPointer<const User> getLoggedInUser()
      { return loggedInUser; }

    inline int getId() const { return id; }

    inline QString getUsername() const { return username; }

    inline QString getRealName() const { return realName; }

    inline QString getDisplayName() const
      { return realName.isEmpty() ? username : realName; }

    virtual ~User();

  private:

    static QSharedPointer<const User> createUserFromRecord
      (const QSqlRecord& record);

    static QVector<QSharedPointer<const User> >
      createUsersFromQueryResults(QSqlQuery& query);

    User(int id, const QString& username, const QString& realName,
         const QString& pwSHA1_hex);

    bool checkPassword(const QString& password) const;

    int id;
    QString username;
    QString realName;
    QString pwSHA1_hex;

    static QMap<int, QWeakPointer<User> > userCache;
    static QSharedPointer<User> loggedInUser;
};

#endif /* USER_H_ */
