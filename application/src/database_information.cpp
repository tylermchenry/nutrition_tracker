#include "database_information.h"
#include <QMessageBox>
#include <QSqlError>
#include <QDebug>
#include <QSettings>
#include <cstring>

DatabaseInformation::DatabaseInformation(QWidget *parent)
    : QDialog(parent)
{
	ui.setupUi(this);

	QSettings settings("Nerdland", "Nutrition Tracker");

	settings.beginGroup("db");
	ui.txtServer->setText(settings.value("server").toString());
	ui.txtDatabase->setText(settings.value("database").toString());
	ui.txtUsername->setText(settings.value("username").toString());
	ui.txtPassword->setText(unfrobPassword(settings.value("password").toByteArray()));
	settings.endGroup();

    connect(ui.btnConnect, SIGNAL(clicked()), this, SLOT(makeConnection()));
    connect(ui.btnExit, SIGNAL(clicked()), this, SLOT(close()));
}

DatabaseInformation::~DatabaseInformation()
{
}

QSqlDatabase DatabaseInformation::getDatabase() const
{
  return QSqlDatabase::database("nutrition_db");
}

bool DatabaseInformation::tryAutoConnect()
{
   if (ui.txtServer->text() != "" && ui.txtDatabase->text() != "" &&
       ui.txtUsername->text() != "" && ui.txtPassword->text() != "")
  {
    makeConnection();
    return getDatabase().isOpen();
  } else {
    return false;
  }
}

void DatabaseInformation::makeConnection()
{
  if (ui.txtServer->text() == "") {
    QMessageBox::critical(this, "Information Missing", "You must enter a server address.");
    return;
  }

  if (ui.txtDatabase->text() == "") {
    QMessageBox::critical(this, "Information Missing", "You must enter a database name.");
    return;
  }

  if (ui.txtUsername->text() == "") {
    QMessageBox::critical(this, "Information Missing", "You must enter a username.");
    return;
  }

  if (ui.txtPassword->text() == "") {
    QMessageBox::critical(this, "Information Missing", "You must enter a password.");
    return;
  }

  ui.txtServer->setEnabled(false);
  ui.txtDatabase->setEnabled(false);
  ui.txtUsername->setEnabled(false);
  ui.txtPassword->setEnabled(false);
  ui.btnConnect->setEnabled(false);
  ui.btnExit->setEnabled(false);

  ui.lblFailed->setText("Connecting...");

  QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL", "nutrition_db");

  db.setHostName(ui.txtServer->text());
  db.setUserName(ui.txtUsername->text());
  db.setPassword(ui.txtPassword->text());
  db.setDatabaseName(ui.txtDatabase->text());

  if (db.open()) {

    QSettings settings("Nerdland", "Nutrition Tracker");
    settings.beginGroup("db");

    if (ui.chkRemember->isChecked()) {
      settings.setValue("server", ui.txtServer->text());
      settings.setValue("database", ui.txtDatabase->text());
      settings.setValue("username", ui.txtUsername->text());
      settings.setValue("password", frobPassword(ui.txtPassword->text()));
    } else {
      settings.remove(""); // Remove all settings in db group
    }

    settings.endGroup();

    this->close();

  } else {

    ui.lblFailed->setText("Connection failed: " + db.lastError().text());

    ui.txtServer->setEnabled(true);
    ui.txtDatabase->setEnabled(true);
    ui.txtUsername->setEnabled(true);
    ui.txtPassword->setEnabled(true);
    ui.btnConnect->setEnabled(true);
    ui.btnExit->setEnabled(true);

    db.close();
  }
}

QByteArray DatabaseInformation::frobPassword(const QString& pw)
{
  QByteArray pwByteArray = pw.toUtf8();
  return frob(pwByteArray);
}

QString DatabaseInformation::unfrobPassword(QByteArray fpwByteArray)
{
  frob(fpwByteArray);
  return QString::fromUtf8(fpwByteArray.data(), fpwByteArray.size());
}

QByteArray& DatabaseInformation::frob(QByteArray& byteArray)
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

