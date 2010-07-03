/*
 * user.h
 * Part of nutrition_tracker
 *
 *  Created on: Jun 26, 2010
 *      Author: Tyler McHenry <tyler@nerdland.net>
 */

#ifndef USER_IMPL_H_
#define USER_IMPL_H_

#include "libnutrition/data/user.h"
#include <QString>
#include <QVector>
#include <QSharedPointer>

class UserImpl : virtual public User
{
  public:

    UserImpl(int id, const QString& username, const QString& realName,
              const QString& pwSHA1_hex);

    virtual ~UserImpl();

    virtual inline int getId() const { return id; }

    virtual inline QString getUsername() const { return username; }

    virtual inline QString getRealName() const { return realName; }

    virtual inline QString getDisplayName() const
      { return realName.isEmpty() ? username : realName; }

    virtual inline void setRealName(const QString& newRealName)
      { realName = newRealName; }

    virtual void setPassword(const QString& newPassword);

    void saveToDatabase();

    // The following methods are only in the Impl class and are not accessible
    // through the interface. They exist for the benefit of the back ends:

    QString getPwSHA1_hex() const { return pwSHA1_hex; }

  protected:

    virtual bool checkPassword(const QString& password) const;

  private:

    int id;
    QString username;
    QString realName;
    QString pwSHA1_hex;
};

#endif /* USER_IMPL_H_ */
