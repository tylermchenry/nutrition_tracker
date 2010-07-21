/*
 * client_connection.cpp
 *
 *  Created on: Jul 20, 2010
 *      Author: tyler
 */

#include "client_connection.h"
#include "libnutrition/proto/service/data_messages.pb.h"
#include "servers/data_server.h"
#include "servers/login_server.h"
#include <cassert>
#include <arpa/inet.h>

ClientConnection::ClientConnection(QTcpSocket* socket, QObject* parent)
  : QObject(parent), tcpSocket(socket), loggedInUserId(-1)
{
  socket->setParent(this);

  resetFields();

  connect(tcpSocket, SIGNAL(disconnected()), this, SLOT(socketDisconnected()));

  connect(tcpSocket, SIGNAL(disconnected()), tcpSocket, SLOT(deleteLater()));

  connect(tcpSocket, SIGNAL(readyRead()), this, SLOT(readData()));
}

ClientConnection::~ClientConnection()
{
}

int ClientConnection::getSocketDescriptor() const
{
  if (tcpSocket) {
    return tcpSocket->socketDescriptor();
  } else {
    return -1;
  }
}

void ClientConnection::readData()
{
  while (tcpSocket->bytesAvailable() > 0) {

    if (currentField == Fields::None) {
      currentField = Fields::MessageNameLength;
    }

    int bytesToRead = computeBytesToRead();

    assert(bytesToRead > 0);

    partialData += tcpSocket->read(bytesToRead);

    if (computeBytesToRead() <= 0) {
      // Finished the previous field
      switch (currentField) {
        case Fields::None:
        case Fields::MessageNameLength:
          messageNameLength = ntohl(*reinterpret_cast<quint32*>(partialData.data()));
          currentField = Fields::MessageName;
          break;
        case Fields::MessageName:
          messageName = QString::fromUtf8(partialData.data(), partialData.length());
          currentField = Fields::ProtocolBufferLength;
          break;
        case Fields::ProtocolBufferLength:
          protocolBufferLength = ntohl(*reinterpret_cast<quint32*>(partialData.data()));
          currentField = Fields::ProtocolBuffer;
          break;
        case Fields::ProtocolBuffer:
          qDebug() << "Received a protobuffer named " << messageName
                    << ", length " << protocolBufferLength;
          assert(partialData.length() == protocolBufferLength);
          handleProtocolBuffer(messageName, partialData);
          currentField = Fields::None;
          break;
      }

      partialData.clear();
    }
  }
}

void ClientConnection::socketDisconnected()
{
  tcpSocket = NULL;
  emit disconnected();
}

void ClientConnection::resetFields()
{
  currentField = Fields::None;
  messageNameLength = 0;
  messageName = "";
  protocolBufferLength = 0;
}

int ClientConnection::computeBytesToRead() const
{
  switch (currentField) {
    case Fields::None:
    case Fields::MessageNameLength:
      return sizeof(quint32) - partialData.length();
    case Fields::MessageName:
      return messageNameLength - partialData.length();
    case Fields::ProtocolBufferLength:
      return sizeof(quint32) - partialData.length();
    case Fields::ProtocolBuffer:
      return protocolBufferLength - partialData.length();
  }

  return 0;
}

void ClientConnection::handleProtocolBuffer
  (const QString& name, const QByteArray& data)
{
  if (name == pbName<DataLoadRequest>()) {
    DataLoadRequest req = parseProtocolBuffer<DataLoadRequest>(data);
    writeMessage(DataServer::loadData(req).serialize());
  } else if (name == pbName<LogInRequest>()) {
    LogInRequest req = parseProtocolBuffer<LogInRequest>(data);
    writeMessage(LoginServer::doLogin(req, loggedInUserId));
  }
}

void ClientConnection::writeMessageLength(quint32 length)
{
  quint32 nbo_length = htonl(length);
  tcpSocket->write(reinterpret_cast<char*>(&nbo_length), sizeof(nbo_length));
}

void ClientConnection::writeMessage(const ::google::protobuf::Message& msg)
{
  qDebug() << "Sending " << QString::fromStdString(msg.GetTypeName()) << " message:";
  qDebug() << QString::fromStdString(msg.DebugString());

  // Temporary simple protocol:
  // [type-length] [type-name] [pb-length] [protocol-buffer]
  // Where lengths are 32-bit unsigned integers in network byte order

  std::string typeName = msg.GetTypeName();
  writeMessageLength(typeName.length());
  tcpSocket->write(typeName.c_str(), typeName.length());
  writeMessageLength(msg.ByteSize());

  // It is necessary to flush the socket before writing the protobuf itself
  // because QTcpSocket does its own internal buffering, and when writing
  // the protobuf, we go around that directly to the descriptor. Without a
  // flush, this can result in the protobuf being written to the network
  // ahead of the header information.

  tcpSocket->flush();
  msg.SerializeToFileDescriptor(getSocketDescriptor());
}

template <typename T> QString ClientConnection::pbName()
{
  return QString::fromStdString(T().GetTypeName());
}

template <typename T> T ClientConnection::parseProtocolBuffer(const QByteArray& data)
{
  T buf;

  buf.ParseFromArray(data.data(), data.length());

  qDebug() << "Received " << QString::fromStdString(buf.GetTypeName()) << " message:";
  qDebug() << QString::fromStdString(buf.DebugString());

  return buf;
}
