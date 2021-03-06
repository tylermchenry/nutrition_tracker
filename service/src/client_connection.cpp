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
#include "servers/nutrient_server.h"
#include "servers/unit_server.h"
#include "servers/group_server.h"
#include "servers/single_food_server.h"
#include "servers/composite_food_server.h"
#include "servers/template_server.h"
#include "servers/meal_server.h"
#include "servers/search_server.h"
#include <QByteArray>
#include <cassert>

#ifdef WIN32
  #include <winsock2.h>
#else
  #include <arpa/inet.h>
#endif

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
  User::setLoggedInUserId(loggedInUserId);

  if (name == pbName<DataLoadRequest>()) {

    DataLoadRequest req = parseProtocolBuffer<DataLoadRequest>(data);
    DataLoadResponseObjects resp_objs = DataServer::loadData(req);
    writeMessage(resp_objs.serialize());

  } else if (name == pbName<LogInRequest>()) {

    User::setLoggedInUserId(-1);

    LogInRequest req = parseProtocolBuffer<LogInRequest>(data);
    writeMessage(LoginServer::doLogin(req, loggedInUserId));

  } else if (name == pbName<NutrientLoadRequest>()) {

    NutrientLoadRequest req = parseProtocolBuffer<NutrientLoadRequest>(data);
    writeMessage(NutrientServer::loadNutrients(req).serialize());

  } else if (name == pbName<UnitLoadRequest>()) {

    UnitLoadRequest req = parseProtocolBuffer<UnitLoadRequest>(data);
    writeMessage(UnitServer::loadUnits(req).serialize());

  } else if (name == pbName<SpecializedUnitLoadRequest>()) {

    SpecializedUnitLoadRequest req = parseProtocolBuffer<SpecializedUnitLoadRequest>(data);
    writeMessage(SpecializedUnitServer::loadSpecializedUnits(req).serialize());

  } else if (name == pbName<GroupLoadRequest>()) {

    GroupLoadRequest req = parseProtocolBuffer<GroupLoadRequest>(data);
    writeMessage(GroupServer::loadGroups(req).serialize());

  } else if (name == pbName<SingleFoodLoadRequest>()) {

    SingleFoodLoadRequest req = parseProtocolBuffer<SingleFoodLoadRequest>(data);
    if (req.nameandidonly()) {
      writeMessage(SingleFoodServer::loadSingleFoodNames(req).serialize());
    } else {
      writeMessage(SingleFoodServer::loadSingleFoods(req).serializeSingleFoods());
    }

  } else if (name == pbName<SingleFoodStoreRequest>()) {

    SingleFoodStoreRequest req = parseProtocolBuffer<SingleFoodStoreRequest>(data);
    writeMessage(SingleFoodServer::storeSingleFoods(req).serialize());

  } else if (name == pbName<SingleFoodDeleteRequest>()) {

    SingleFoodDeleteRequest req = parseProtocolBuffer<SingleFoodDeleteRequest>(data);
    writeMessage(SingleFoodServer::deleteSingleFoods(req).serialize());

  } else if (name == pbName<CompositeFoodLoadRequest>()) {

    CompositeFoodLoadRequest req = parseProtocolBuffer<CompositeFoodLoadRequest>(data);
    if (req.nameandidonly()) {
      writeMessage(CompositeFoodServer::loadCompositeFoodNames(req).serialize());
    } else {
      writeMessage(CompositeFoodServer::loadCompositeFoods(req).serializeCompositeFoods());
    }

  } else if (name == pbName<CompositeFoodStoreRequest>()) {

    CompositeFoodStoreRequest req = parseProtocolBuffer<CompositeFoodStoreRequest>(data);
    writeMessage(CompositeFoodServer::storeCompositeFoods(req).serialize());

  } else if (name == pbName<CompositeFoodDeleteRequest>()) {

    CompositeFoodDeleteRequest req = parseProtocolBuffer<CompositeFoodDeleteRequest>(data);
    writeMessage(CompositeFoodServer::deleteCompositeFoods(req).serialize());

  } else if (name == pbName<TemplateLoadRequest>()) {

    TemplateLoadRequest req = parseProtocolBuffer<TemplateLoadRequest>(data);
    if (req.nameandidonly()) {
      writeMessage(TemplateServer::loadTemplateNames(req).serialize());
    } else {
      writeMessage(TemplateServer::loadTemplates(req).serializeTemplates());
    }

  } else if (name == pbName<TemplateStoreRequest>()) {

    TemplateStoreRequest req = parseProtocolBuffer<TemplateStoreRequest>(data);
    writeMessage(TemplateServer::storeTemplates(req).serialize());

  } else if (name == pbName<TemplateDeleteRequest>()) {

    TemplateDeleteRequest req = parseProtocolBuffer<TemplateDeleteRequest>(data);
    writeMessage(TemplateServer::deleteTemplates(req).serialize());

  } else if (name == pbName<MealLoadRequest>()) {

    MealLoadRequest req = parseProtocolBuffer<MealLoadRequest>(data);
    if (req.nameandidonly()) {
      writeMessage(MealServer::loadMealNames(req).serialize());
    } else {
      writeMessage(MealServer::loadMeals(req).serialize());
    }

  } else if (name == pbName<MealStoreRequest>()) {

    MealStoreRequest req = parseProtocolBuffer<MealStoreRequest>(data);
    writeMessage(MealServer::storeMeals(req).serialize());

  } else if (name == pbName<MealDeleteRequest>()) {

    MealDeleteRequest req = parseProtocolBuffer<MealDeleteRequest>(data);
    writeMessage(MealServer::deleteMeals(req).serialize());

  } else if (name == pbName<UserLoadRequest>()) {

    UserLoadRequest req = parseProtocolBuffer<UserLoadRequest>(data);
    writeMessage(UserServer::loadUsers(req).serialize());

  } else if (name == pbName<UserStoreRequest>()) {

    UserStoreRequest req = parseProtocolBuffer<UserStoreRequest>(data);
    writeMessage(UserServer::storeUsers(req).serialize());

  } else if (name == pbName<SearchRequest>()) {

    SearchRequest req = parseProtocolBuffer<SearchRequest>(data);
    writeMessage(SearchServer::searchFoods(req).serialize());

  }
}

void ClientConnection::writeMessageLength(quint32 length)
{
  quint32 nbo_length = htonl(length);
  tcpSocket->write(reinterpret_cast<char*>(&nbo_length), sizeof(nbo_length));
}

void ClientConnection::writeMessage(const ::google::protobuf::Message& msg)
{
#ifdef VERBOSE_DEBUG
  qDebug() << "Sending " << QString::fromStdString(msg.GetTypeName()) << " message:";
  qDebug() << QString::fromStdString(msg.DebugString());
#endif

  // Temporary simple protocol:
  // [type-length] [type-name] [pb-length] [protocol-buffer]
  // Where lengths are 32-bit unsigned integers in network byte order

  std::string typeName = msg.GetTypeName();
  writeMessageLength(typeName.length());
  tcpSocket->write(typeName.c_str(), typeName.length());
  writeMessageLength(msg.ByteSize());

  QByteArray message(msg.ByteSize(), '\0');

  msg.SerializeToArray(message.data(), message.length());
  tcpSocket->write(message.data(), message.length());
}

template <typename T> QString ClientConnection::pbName()
{
  return QString::fromStdString(T().GetTypeName());
}

template <typename T> T ClientConnection::parseProtocolBuffer(const QByteArray& data)
{
  T buf;

  buf.ParseFromArray(data.data(), data.length());

#ifdef VERBOSE_DEBUG
  qDebug() << "Received " << QString::fromStdString(buf.GetTypeName()) << " message:";
  qDebug() << QString::fromStdString(buf.DebugString());
#endif

  return buf;
}
