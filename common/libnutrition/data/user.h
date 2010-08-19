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

class UserData; // forward decl

class User
{
  public:

    // Definitions to make this class cacheable with DataCache
    typedef int cache_key_type;
    typedef User cache_object_type;
    typedef QString cache_sort_key_type;
    static QString (User::* const cache_get_sort_key)() const;
    static const bool cache_strong = false;

    static QSharedPointer<User> getUser(int id);

    static QSharedPointer<User> getUserByUsername
      (const QString& username);

    static QVector<QSharedPointer<User> > getAllUsers();

    static QSharedPointer<User> createUser
      (const QString& username, const QString& password,
       const QString& realName, QString& errorMessage);

    static QSharedPointer<User> createUserWithPasswordHash
      (const QString& username, const QString& password_hash,
       const QString& realName, QString& errorMessage);

    static QSharedPointer<User> logInAs
      (const QString& username, const QString& password,
       QString& errorMessage);

    // This function does not validate the user's password. It is simply a
    // bare assertion that the logged in user should be a particular ID. This
    // is intended for use by the service backend, and will cause errors to be
    // returned from the service if it used from the front-end instead of the
    // proper logInAs function.
    inline static void setLoggedInUserId(int userId)
      { loggedInUser = getUser(userId); }

    inline static int getLoggedInUserId()
      { return loggedInUser ? loggedInUser->getId() : -1; }

    inline static QSharedPointer<User> getLoggedInUser()
      { return loggedInUser; }

    virtual ~User();

    virtual int getId() const = 0;

    virtual QString getUsername() const = 0;

    virtual QString getRealName() const = 0;

    virtual QString getDisplayName() const = 0;

    virtual void setRealName(const QString& newRealName) = 0;

    virtual void setPassword(const QString& newPassword) = 0;

    virtual bool checkPassword(const QString& password) const = 0;

    virtual bool checkPasswordHash(const QString& password_hash) const = 0;

    virtual void saveToDatabase() = 0;

    virtual UserData serialize(bool sanitized = false) const = 0;

  private:

    static QSharedPointer<User> loggedInUser;

    friend class UserImpl;
};

#endif /* USER_H_ */
