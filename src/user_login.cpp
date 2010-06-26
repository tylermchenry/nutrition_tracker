#include "user_login.h"
#include "data/user.h"
#include <QSettings>
#include <QtSql/QSqlQuery>
#include <QtGui/QMessageBox>

UserLogin::UserLogin(QWidget *parent)
  : QDialog(parent), bLoggedIn(false), bCreate(false)
{
  ui.setupUi(this);

  QSettings settings("Nerdland", "Nutrition Tracker");

  settings.beginGroup("user");
  ui.txtUsername->setText(settings.value("username").toString());
  ui.txtPassword->setText(unfrobPassword(settings.value("password").toByteArray()));
  settings.endGroup();

  toggleCreate(Qt::Unchecked);
  connect(ui.chkNewAccount, SIGNAL(stateChanged(int)), this, SLOT(toggleCreate(int)));

  connect(ui.btnLogIn, SIGNAL(clicked()), this, SLOT(logIn()));
  connect(ui.btnExit, SIGNAL(clicked()), this, SLOT(close()));
}

UserLogin::~UserLogin()
{
}

bool UserLogin::tryAutoLogIn()
{
  if (ui.txtUsername->text() != "" && ui.txtPassword->text() != "") {
    logIn();
    return loggedIn();
  } else {
    return false;
  }
}

void UserLogin::toggleCreate(int state)
{
  bCreate = (state == Qt::Checked);

  ui.lblConfirm->setVisible(bCreate);
  ui.txtConfirm->setVisible(bCreate);
  ui.lblRealName->setVisible(bCreate);
  ui.txtRealName->setVisible(bCreate);
}

void UserLogin::logIn()
{
  if (ui.txtUsername->text() == "") {
    QMessageBox::warning(this, "Login Failure", "You must enter a username.");
  } else if (ui.txtPassword->text() == "") {
    QMessageBox::warning(this, "Login Failure", "You must enter a password.");
  } else if (!bCreate) {

    QSharedPointer<const User> user = User::getUserByUsername(ui.txtUsername->text());

    if (user) {
      QString errorMessage;
      if (User::logInAs(user, ui.txtPassword->text(), errorMessage)) {
        success();
      } else {
        QMessageBox::warning(this, "Login Failure", "Cannot log in: " + errorMessage);
      }
    } else {
      QMessageBox::warning
        (this, "Login Failure",
         "Cannot log in: The username you entered does not exist.");
    }

  } else if (ui.txtConfirm->text() == "") {
    QMessageBox::warning(this, "Login Failure", "You must confirm your password.");
  } else if (ui.txtConfirm->text() != ui.txtPassword->text()) {
    QMessageBox::warning(this, "Login Failure", "Passwords do not match.");
  } else {

    QString errorMessage;
    QSharedPointer<const User> user = User::createUser
      (ui.txtUsername->text(), ui.txtPassword->text(), ui.txtRealName->text(),
       errorMessage);

    if (user) {
      if (User::logInAs(user, ui.txtPassword->text(), errorMessage)) {
        success();
      } else {
        QMessageBox::warning(this, "Login Failure", "Cannot log in: " + errorMessage);
      }
    } else {
      QMessageBox::warning
        (this, "Login Failure", "Cannot create user: " + errorMessage);
    }
  }
}

void UserLogin::success()
{
  bLoggedIn = true;

  QSettings settings("Nerdland", "Nutrition Tracker");
  settings.beginGroup("user");

  if (ui.chkRemember->isChecked()) {
    settings.setValue("username", ui.txtUsername->text());
    settings.setValue("password", frobPassword(ui.txtPassword->text()));
  } else {
    settings.remove("username");
    settings.remove("password");
  }

  settings.endGroup();

  accept();
}

QByteArray UserLogin::frobPassword(const QString& pw)
{
  QByteArray pwByteArray = pw.toUtf8();
  return frob(pwByteArray);
}

QString UserLogin::unfrobPassword(QByteArray fpwByteArray)
{
  frob(fpwByteArray);
  return QString::fromUtf8(fpwByteArray.data(), fpwByteArray.size());
}

QByteArray& UserLogin::frob(QByteArray& byteArray)
{
  // This is a means of *obfuscation*, not *encryption*. This is a trivial,
  // symmetric mangling of the user's password so that it is not stored in
  // the configuration file in cleartext. This does not provide security
  // for the password in the configuration file; it just makes it slightly
  // harder to casually observe the password.

  for (int i = 0; i < byteArray.size(); ++i) {
    byteArray[i] = byteArray[i] ^ 42;
  }

  return byteArray;
}
