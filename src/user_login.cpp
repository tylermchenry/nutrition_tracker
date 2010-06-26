#include "user_login.h"
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

    QSqlDatabase db = QSqlDatabase::database("nutrition_db");
    QSqlQuery query(db);

    query.prepare("SELECT * FROM user WHERE User_Name=:username AND"
        " PW_SHA1=SHA1(CONCAT(:password, :username2)) LIMIT 1");
    query.bindValue(":username", ui.txtUsername->text());
    query.bindValue(":username2", ui.txtUsername->text());
    query.bindValue(":password", ui.txtPassword->text());

    if (query.exec() && query.first()) {
      success();
    } else {
      QMessageBox::warning
        (this, "Login Failure", "The username or password you entered was not valid.");
    }

  } else if (ui.txtConfirm->text() == "") {
    QMessageBox::warning(this, "Login Failure", "You must confirm your password.");
  } else if (ui.txtConfirm->text() != ui.txtPassword->text()) {
    QMessageBox::warning(this, "Login Failure", "Passwords do not match.");
  } else {

    QSqlDatabase db = QSqlDatabase::database("nutrition_db");
    QSqlQuery query(db);

    query.prepare("SELECT * FROM user WHERE User_Name=:username LIMIT 1");
    query.bindValue(":username", ui.txtUsername->text());

    if (query.exec() && query.first()) {
      QMessageBox::warning
        (this, "Login Failure", "The username you entered is already in use.");
      return;
    }

    query.prepare("INSERT INTO user (User_Id, User_Name, Name, PW_SHA1) "
                  " VALUES (NULL, :username, :name, "
                  "         SHA1(CONCAT(:password, :username2)))");

    query.bindValue(":username", ui.txtUsername->text());
    query.bindValue(":username2", ui.txtUsername->text());
    query.bindValue(":name", ui.txtRealName->text());
    query.bindValue(":password", ui.txtPassword->text());

    if (query.exec()) {
      success();
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
