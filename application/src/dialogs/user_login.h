#ifndef USER_LOGIN_H
#define USER_LOGIN_H

#include <QtGui/QDialog>
#include <QByteArray>
#include "ui_user_login.h"

class UserLogin : public QDialog
{
    Q_OBJECT

  public:

    UserLogin(QWidget *parent = 0);

    ~UserLogin();

    inline bool loggedIn() { return bLoggedIn; }

    bool tryAutoLogIn();

  public slots:

    void toggleCreate(int state);

    void logIn();

  private:

    Ui::UserLoginUI ui;

    bool bLoggedIn;
    bool bCreate;

    void success();

    static QByteArray frobPassword(const QString& pw);
    static QString unfrobPassword(QByteArray fpwByteArray);
    static QByteArray& frob(QByteArray& byteArray);

};

#endif // USER_LOGIN_H
