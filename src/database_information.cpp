#include "database_information.h"
#include <QMessageBox>
#include <QSqlError>
#include <QDebug>

DatabaseInformation::DatabaseInformation(QWidget *parent)
    : QDialog(parent)
{
	ui.setupUi(this);

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
