/*
 * client_connection.h
 *
 *  Created on: Jul 20, 2010
 *      Author: tyler
 */

#ifndef CLIENT_CONNECTION_H_
#define CLIENT_CONNECTION_H_

#include <QObject>
#include <QByteArray>
#include <QtNetwork/QTcpSocket>
#include <google/protobuf/message.h>
#include <google/protobuf/repeated_field.h>

class ClientConnection : public QObject
{
  Q_OBJECT

  public:

    explicit ClientConnection(QTcpSocket* socket, QObject* parent = NULL);

    virtual ~ClientConnection();

    int getSocketDescriptor() const;

  signals:

    void disconnected();

  private slots:

    void readData();
    void socketDisconnected();

  private:

    struct Fields {
      enum Field {
        None,
        MessageNameLength,
        MessageName,
        ProtocolBufferLength,
        ProtocolBuffer
      };
    };

    QTcpSocket* tcpSocket;
    QByteArray partialData;

    Fields::Field currentField;

    int messageNameLength;
    QString messageName;
    int protocolBufferLength;

    int loggedInUserId;

    void resetFields();

    int computeBytesToRead() const;

    void handleProtocolBuffer(const QString& name, const QByteArray& data);

    void writeMessageLength(quint32 length);

    void writeMessage(const ::google::protobuf::Message& msg);

    template <typename T> static QString pbName();

    template <typename T> T parseProtocolBuffer(const QByteArray& data);
};

#endif /* CLIENT_CONNECTION_H_ */
