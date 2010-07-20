/*
 * nutrition_server_application.cpp
 * Part of nutritiond
 *
 *  Created on: Jul 20, 2010
 *      Author: Tyler McHenry <tyler@nerdland.net>
 */

#include "nutrition_server_application.h"
#include <QDebug>

NutritionServerApplication::NutritionServerApplication(int &c, char **v)
  : QCoreApplication(c, v)
{
  tcpServer = new QTcpServer(this);

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
