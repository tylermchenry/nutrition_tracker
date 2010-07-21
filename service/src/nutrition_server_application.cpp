/*
 * nutrition_server_application.cpp
 * Part of nutritiond
 *
 *  Created on: Jul 20, 2010
 *      Author: Tyler McHenry <tyler@nerdland.net>
 */

#include "nutrition_server_application.h"
#include "libnutrition/backend/mysql/mysql_back_end.h"
#include <QtSql/QSqlDatabase>
#include <QDebug>
#include <stdexcept>

NutritionServerApplication::NutritionServerApplication(int &c, char **v)
  : QCoreApplication(c, v)
{
  tcpServer = new QTcpServer(this);

  // TODO: Configurable
  QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL", "nutrition_db");

  db.setHostName("localhost");
  db.setUserName("root");
  db.setPassword("testpassword");
  db.setDatabaseName("nutrition");

  if (!db.open()) {
    delete tcpServer;
    throw std::runtime_error("Could not connect to database");
  }

  BackEnd::setBackEnd(QSharedPointer<MySQLBackEnd>(new MySQLBackEnd("nutrition_db")));

  if (!tcpServer->listen(QHostAddress::Any, 2133)) {
    qDebug() << "Server failed to start.";
  } else {
    connect(tcpServer, SIGNAL(newConnection()), this, SLOT(acceptNewConnection()));
  }
}

NutritionServerApplication::~NutritionServerApplication()
{
}

void NutritionServerApplication::acceptNewConnection()
{
  QSharedPointer<ClientConnection> connection
    (new ClientConnection(tcpServer->nextPendingConnection(), this));
  connections.insert(connection->getSocketDescriptor(), connection);
}
