/*
 * nutrition_server_application.h
 * Part of nutritiond
 *
 *  Created on: Jul 20, 2010
 *      Author: Tyler McHenry <tyler@nerdland.net>
 */

#ifndef NUTRITION_SERVER_APPLICATION_H_
#define NUTRITION_SERVER_APPLICATION_H_

#include <QCoreApplication>
#include "client_connection.h"
#include <QSharedPointer>
#include <QMap>
#include <QtNetwork/QTcpServer>

class NutritionServerApplication : public QCoreApplication
{
  Q_OBJECT;

  public:

    NutritionServerApplication(int &c, char **v);

    virtual ~NutritionServerApplication();

  private slots:

    void acceptNewConnection();

  private:

    QTcpServer* tcpServer;
    QMap<int, QSharedPointer<ClientConnection> > connections;
};

#endif /* NUTRITION_SERVER_APPLICATION_H_ */
