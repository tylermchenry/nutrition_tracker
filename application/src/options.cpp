#include "options.h"
#include "data/user.h"
#include <QSettings>
#include <QtSql/QSqlDatabase>
#include <QtGui/QMessageBox>

Options::Options(QWidget *parent)
    : QDialog(parent)
{
  ui.setupUi(this);

  connect(ui.chkSaveConnection, SIGNAL(stateChanged(int)),
          this, SLOT(toggleDBOptions(int)));

  connect(ui.btnCancel, SIGNAL(clicked()), this, SLOT(reject()));
  connect(ui.btnApply, SIGNAL(clicked()), this, SLOT(saveOptions()));
  connect(ui.btnOK, SIGNAL(clicked()), this, SLOT(saveOptionsAndClose()));

  loadOptions();
}

Options::~Options()
{

}

void Options::saveOptionsAndClose()
{
  if (saveOptions()) {
    accept();
  }
}

bool Options::saveOptions()
{
  QSettings settings("Nerdland", "Nutrition Tracker");

  settings.beginGroup("user");
  if (ui.txtUserPassword->text() != unfrobPassword(settings.value("password").toByteArray())) {
    if (ui.txtUserConfirm->text().isEmpty()) {
      QMessageBox::warning
        (this, "Password Change", "You must re-enter your new password to confirm the change.");
      return false;
    } else if (ui.txtUserConfirm->text() != ui.txtUserPassword->text()) {
      QMessageBox::warning(this, "Password Change", "Passwords do not match.");
      return false;
    } else {
      User::getLoggedInUser()->setPassword(ui.txtUserPassword->text());
    }
  }
  User::getLoggedInUser()->setRealName(ui.txtUserRealName->text());
  User::getLoggedInUser()->saveToDatabase();

  if (!ui.chkSaveAccount->isChecked()) {
    settings.remove("");
  } else {
    settings.setValue("username", ui.txtUserUsername->text());
    settings.setValue("password", frobPassword(ui.txtUserPassword->text()));
  }
  settings.endGroup();

  settings.beginGroup("db");
  if (!ui.chkSaveConnection->isChecked()) {
    settings.remove("");
  } else {

    bool changed =
      ui.txtDBServer->text() != settings.value("server").toString() ||
      ui.txtDBDatabase->text() != settings.value("database").toString() ||
      ui.txtDBUsername->text() != settings.value("username").toString() ||
      ui.txtDBPassword->text() != unfrobPassword(settings.value("password").toByteArray());

    if (changed) {
      settings.setValue("server", ui.txtDBServer->text());
      settings.setValue("database", ui.txtDBDatabase->text());
      settings.setValue("username", ui.txtDBUsername->text());
      settings.setValue("password", frobPassword(ui.txtDBPassword->text()));

      QMessageBox::information
        (this, "Database connection changed",
         "New database connection settings will not take effect until you restart Nutrition Tracker.");
    }
  }
  settings.endGroup();

  settings.beginGroup("preferences");
  settings.setValue("includerefuse", ui.chkDefaultToIncludeRefuse->isChecked());
  settings.endGroup();

  return true;
}

void Options::toggleDBOptions(int state)
{
  bool saveConnection = (state == Qt::Checked);

  ui.txtDBServer->setEnabled(saveConnection);
  ui.txtDBDatabase->setEnabled(saveConnection);
  ui.txtDBUsername->setEnabled(saveConnection);
  ui.txtDBPassword->setEnabled(saveConnection);
}

void Options::loadOptions()
{
  QSqlDatabase db = QSqlDatabase::database("nutrition_db");
  QSettings settings("Nerdland", "Nutrition Tracker");

  settings.beginGroup("db");
  ui.chkSaveConnection->setChecked(settings.contains("server"));
  ui.txtDBServer->setText(settings.value("server", db.hostName()).toString());
  ui.txtDBDatabase->setText(settings.value("database", db.databaseName()).toString());
  ui.txtDBUsername->setText(settings.value("username", db.userName()).toString());
  ui.txtDBPassword->setText(unfrobPassword(settings.value("password", frobPassword(db.password())).toByteArray()));
  settings.endGroup();

  settings.beginGroup("user");
  ui.chkSaveAccount->setChecked(settings.contains("username"));
  ui.txtUserUsername->setText(User::getLoggedInUser()->getUsername());
  ui.txtUserRealName->setText(User::getLoggedInUser()->getRealName());
  ui.txtUserPassword->setText(unfrobPassword(settings.value("password").toByteArray()));
  settings.endGroup();

  settings.beginGroup("preferences");
  ui.chkDefaultToIncludeRefuse->setChecked(settings.value("includerefuse", true).toBool());
  settings.endGroup();
}

QByteArray Options::frobPassword(const QString& pw)
{
  QByteArray pwByteArray = pw.toUtf8();
  return frob(pwByteArray);
}

QString Options::unfrobPassword(QByteArray fpwByteArray)
{
  frob(fpwByteArray);
  return QString::fromUtf8(fpwByteArray.data(), fpwByteArray.size());
}

QByteArray& Options::frob(QByteArray& byteArray)
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
