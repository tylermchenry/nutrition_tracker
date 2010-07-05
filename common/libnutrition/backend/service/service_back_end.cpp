/*
 * service_back_end.cpp
 * Part of libnutrition
 *
 *  Created on: Jul 4, 2010
 *      Author: Tyler McHenry <tyler@nerdland.net>
 */

#include "service_back_end.h"
#include "libnutrition/proto/data.pb.h"
#include "libnutrition/data/impl/food_collection_impl.h"
#include <string>
#include <stdexcept>
#include <arpa/inet.h>

ServiceBackEnd::ServiceBackEnd(const QString& remoteHost, quint16 port)
  : loadedAllGroups(false), loadedAllUnits(false), loadedAllNutrients(false)
{
  socket.connectToHost(remoteHost, port);

  if (!socket.waitForConnected()) {
    throw std::runtime_error
      ("Could not connect to nutrition service at host " +
        remoteHost.toStdString() + ":" + QString::number(port).toStdString() +
        " as back end.");
  }
}

ServiceBackEnd::~ServiceBackEnd()
{
  socket.disconnectFromHost();
  socket.close();
}

void ServiceBackEnd::setOmissions(DataLoadRequest& req)
{
  if (loadedAllGroups && !req.has_grouploadrequest()) {
    req.mutable_grouploadrequest()->set_omit(true);
  }

  if (loadedAllUnits && !req.has_unitloadrequest()) {
    req.mutable_unitloadrequest()->set_omit(true);
  }

  if (loadedAllNutrients && !req.has_nutrientloadrequest()) {
    req.mutable_nutrientloadrequest()->set_omit(true);
  }

  if (!req.has_userloadrequest()) {
    req.mutable_nutrientloadrequest()->set_omit(true);
  }
}

void ServiceBackEnd::writeMessageLength(quint32 length)
{
  quint32 nbo_length = htonl(length);
  socket.write(reinterpret_cast<char*>(&nbo_length), sizeof(nbo_length));
}

void ServiceBackEnd::writeMessage(const ::google::protobuf::Message& msg)
{
  // Temporary simple protocol:
  // [type-length] [type-name] [pb-length] [protocol-buffer]
  // Where lengths are 32-bit unsigned integers in network byte order

  std::string typeName = msg.GetTypeName();
  writeMessageLength(typeName.length());
  socket.write(typeName.c_str(), typeName.length());
  writeMessageLength(msg.ByteSize());
  msg.SerializeToFileDescriptor(socket.socketDescriptor());
}

quint32 ServiceBackEnd::readResponseLength()
{
  quint32 nbo_length;

  QByteArray bytes;

  while (bytes.size() < static_cast<int>(sizeof(nbo_length))) {
    if (socket.waitForReadyRead(3000)) {
      bytes += socket.read(sizeof(nbo_length) - bytes.size());
    } else {
      throw std::runtime_error("Socket failed or read timed out");
    }
  }

  nbo_length = *reinterpret_cast<quint32*>(bytes.data());

  return ntohl(nbo_length);
}

void ServiceBackEnd::loadResponseData
  (LoadedData& loadedData, const DataLoadResponse& resp)
{
  if (resp.has_error() && resp.error().iserror()) {
    throw std::runtime_error("Service protocol load error: " +
                             resp.error().errormessage());
  }

  // Order here is very important. We need to construct objects whose pointers
  // may be required as parts of other objects first. i.e. Meals require
  // Composite Foods require SingleFoods require Nutrients require Units, so
  // the construction order must be Unit, Nutrient, SingleFood, CompositeFood,
  // Meal.

  // The server guarantees that within a given category, the construction order
  // is appropriate. e.g. if Composite Food A contains an amount of Composite
  // Food B, then we can assume that B appears before A in the list of Composite
  // Food data.

  // Depend on nothing:

  if (resp.has_grouploadresponse()) {
    loadResponseData(loadedData, resp.grouploadresponse());
  }

  if (resp.has_userloadresponse()) {
    loadResponseData(loadedData, resp.userloadresponse());
  }

  if (resp.has_unitloadresponse()) {
    loadResponseData(loadedData, resp.unitloadresponse());
  }

  // Depend on Unit:

  if (resp.has_specializedunitloadresponse()) {
    loadResponseData(loadedData, resp.specializedunitloadresponse());
  }

  if (resp.has_nutrientloadresponse()) {
    loadResponseData(loadedData, resp.nutrientloadresponse());
  }

  // Depend on Nutrient:

  if (resp.has_singlefoodloadresponse()) {
    loadResponseData(loadedData, resp.singlefoodloadresponse());
  }

  // Depend on Single Food:

  if (resp.has_compositefoodloadresponse()) {
    loadResponseData(loadedData, resp.compositefoodloadresponse());
  }

  // Depend on Composite Food:

  if (resp.has_templateloadresponse()) {
    loadResponseData(loadedData, resp.templateloadresponse());
  }

  if (resp.has_mealloadresponse()) {
    loadResponseData(loadedData, resp.mealloadresponse());
  }
}

void ServiceBackEnd::setComponents
  (const QSharedPointer<FoodCollectionImpl>& collection_impl,
   const ::google::protobuf::RepeatedPtrField<FoodComponentData>& componentData)
{
  QList<FoodComponent> components;

  for (int i = 0; i < componentData.size(); ++i) {
    const FoodComponentData& fcdata = componentData.Get(i);

    CompositeFood::ContainedTypes::ContainedType containedType =
      static_cast<CompositeFood::ContainedTypes::ContainedType>(fcdata.foodtype());

    QSharedPointer<Food> food;

    switch (containedType)
    {
      case CompositeFood::ContainedTypes::SingleFood:
        food = SingleFood::getSingleFood(fcdata.foodid());
        break;
      case CompositeFood::ContainedTypes::CompositeFood:
        food = CompositeFood::getCompositeFood(fcdata.foodid());
        break;
      case CompositeFood::ContainedTypes::Template:
        food = Template::getTemplate(fcdata.foodid());
        break;
      default:
        throw std::runtime_error("Server specified a component of an unknown type.");
    }

    FoodAmount famount
      (food,
       fcdata.amount(),
       Unit::getUnit(QString::fromStdString(fcdata.unitabbreviation())),
       fcdata.includesrefuse());

    components.append
      (FoodComponent(collection_impl, fcdata.id(), famount, fcdata.order()));
  }

  collection_impl->setComponents(components);
}