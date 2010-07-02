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

    // Definitions to make this class cacheable with DataCache
    typedef int cache_key_type;
    typedef User cache_object_type;
    typedef QString cache_sort_key_type;
    static QString (User::* const cache_get_sort_key)() const;
    static const bool cache_strong = false;

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

    inline static QSharedPointer<User> getLoggedInUser()
      { return loggedInUser; }

    virtual ~User();

    virtual int getId() const = 0;

    virtual QString getUsername() const = 0;

    virtual QString getRealName() const = 0;

    virtual QString getDisplayName() const = 0;

    virtual void setRealName(const QString& newRealName) = 0;

    virtual void setPassword(const QString& newPassword) = 0;

    virtual void saveToDatabase() = 0;

  protected:

    virtual bool checkPassword(const QString& password) const = 0;

  private:

    static QSharedPointer<const User> createUserFromRecord
      (const QSqlRecord& record);

    static QVector<QSharedPointer<const User> >
      createUsersFromQueryResults(QSqlQuery& query);

    static QSharedPointer<User> loggedInUser;

    friend class UserImpl;
};

#endif /* USER_H_ */
