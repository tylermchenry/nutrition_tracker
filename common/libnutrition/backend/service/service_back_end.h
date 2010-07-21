/*
 * service_back_end.h
 *
 *  Created on: Jul 4, 2010
 *      Author: tyler
 */

#ifndef SERVICE_BACK_END_H_
#define SERVICE_BACK_END_H_

#include "../back_end.h"
#include <google/protobuf/message.h>
#include <google/protobuf/repeated_field.h>
#include <QString>
#include <QByteArray>
#include <QtNetwork/QTcpSocket>
#include <string>
#include <stdexcept>

// Forward declarations of response protocol buffers, so that we don't have to
// #include every protobuf header here

class DataLoadRequest;

class SingleFoodLoadResponse;
class CompositeFoodLoadResponse;
class MealLoadResponse;
class TemplateLoadResponse;
class NutrientLoadResponse;
class UnitLoadResponse;
class SpecializedUnitLoadResponse;
class UserLoadResponse;
class GroupLoadResponse;
class DataLoadResponse;

class FoodComponentData;
class FoodCollectionImpl;

class ServiceBackEnd : public BackEnd
{
  public:

    using BackEnd::FoodTypes;
    using BackEnd::SearchRequest;
    using BackEnd::SearchResult;

    static const quint16 DEFAULT_PORT = 2133;

    explicit ServiceBackEnd
      (const QString& remoteHost, quint16 port = DEFAULT_PORT);

    virtual ~ServiceBackEnd();

    virtual QMultiMap<QString, QPair<FoodTypes::FoodType, int> >
      loadFoodNamesForUser(int userId, bool includeExpired);

    QMultiMap<QString, QPair<FoodTypes::FoodType, int> >
       loadFoodNamesForUser
         (int userId, FoodTypes::FoodType type, bool includeExpired = false)
    { return BackEnd::loadFoodNamesForUser(userId, type, includeExpired); }

    /** Single Foods **/

    virtual QSharedPointer<SingleFood> loadSingleFood(int id);

    virtual QMultiMap<QString, int> loadSingleFoodNamesForUser(int userId);

    virtual void storeSingleFood(const QSharedPointer<SingleFood>& food);

    virtual void deleteSingleFood(const QSharedPointer<SingleFood>& food);

    virtual void deleteSingleFood(int id);


    /** Composite Foods **/

    virtual QSharedPointer<CompositeFood> loadCompositeFood(int id);

    virtual QMultiMap<QString, int> loadCompositeFoodNamesForUser
    (int userId, bool includeExpired);

    virtual void storeCompositeFood(const QSharedPointer<CompositeFood>& food);

    virtual void deleteCompositeFood(const QSharedPointer<CompositeFood>& food);

    virtual void deleteCompositeFood(int id);


    /** Templates **/

    virtual QSharedPointer<Template> loadTemplate(int id);

    virtual QMultiMap<QString, int> loadTemplateNamesForUser(int userId);

    virtual void storeTemplate(const QSharedPointer<Template>& templ);

    virtual void deleteTemplate(const QSharedPointer<Template>& templ);

    virtual void deleteTemplate(int id);


    /** Meals **/

    virtual QSharedPointer<Meal> loadMeal
      (int userId, const QDate& date, int mealId);

    virtual QMap<int, QString> loadAllMealNames();

    virtual QMap<int, QString> loadAllMealNamesForUser
      (int userId, bool includeGenerics);

    virtual QList<QSharedPointer<Meal> > loadAllMealsForDay
      (int userId, const QDate& date);

    virtual void storeMeal(const QSharedPointer<Meal>& meal);

    virtual void deleteMeal(const QSharedPointer<Meal>& templ);

    virtual void deleteMeal(int userId, const QDate& date, int mealId);


    /** Users **/

    virtual QSharedPointer<User> loadUser(int id);

    virtual QSharedPointer<User> loadUserByUsername
      (const QString& username);

    virtual QList<QSharedPointer<User> > loadAllUsers();

    virtual void storeUser(const QSharedPointer<User>& user);


    /** Nutrients **/

    virtual QSharedPointer<const Nutrient> loadNutrient(const QString& id);

    virtual QList<QSharedPointer<Nutrient> > loadAllNutrients();

    virtual QList<QSharedPointer<Nutrient> > loadAllNutrients
       (Nutrient::Categories::Category category);


    /** Units **/

    virtual QSharedPointer<const Unit> loadUnit(const QString& abbr);

    virtual QList<QSharedPointer<Unit> > loadAllUnits();

    virtual QList<QSharedPointer<Unit> > loadAllUnits
      (Unit::Dimensions::Dimension dimension);

    virtual QSharedPointer<const Unit> loadBasicUnit
      (Unit::Dimensions::Dimension dimension);

    virtual QSharedPointer<const SpecializedUnit>
      loadSpecializedUnit(int foodId, int sequence);

    virtual QList<QSharedPointer<SpecializedUnit> >
      loadAllSpecializedUnitsForFood(int foodId);

    virtual QPair<QList<QSharedPointer<Unit> >,
      QList<QSharedPointer<SpecializedUnit> > >
        loadAllUnitsForFood(int foodId);

    /** Groups **/

    virtual QSharedPointer<const Group> loadGroup(const QString& id);

    virtual QList<QSharedPointer<Group> > loadAllGroups();


    /** Login / Register **/

    virtual QSharedPointer<User>
      logIn(const QString& username, const QString& password,
            QString& errorMessage);

    virtual QSharedPointer<User>
      registerAndLogIn(const QString& userName, const QString& realName,
                         const QString& password, QString& errorMessage);

    /** Search **/

    virtual QList<SearchResult> searchFoods(const SearchRequest& request);


  private:

    QTcpSocket socket;

    bool loadedAllGroups;
    bool loadedAllUnits;
    bool loadedAllNutrients;

    void setOmissions(DataLoadRequest& req);

    void writeMessageLength(quint32 length);

    void writeMessage(const ::google::protobuf::Message& msg);

    quint32 readResponseLength();

    template <typename T> void readResponse(T& t);

    template <typename T> void writeMessageAndReadResponse
      (const ::google::protobuf::Message& msg, T& t);

    struct LoadedData
    {
      QMap<int, QSharedPointer<SingleFood> > singleFoods;
      QMap<int, QSharedPointer<CompositeFood> > compositeFoods;
      QMap<int, QSharedPointer<Template> > templates;
      QMap<Meal::MealIdTuple, QSharedPointer<Meal> > meals;
      QMap<int, QSharedPointer<User> > users;
      QMap<QString, QSharedPointer<Nutrient> > nutrients;
      QMap<QString, QSharedPointer<Unit> > units;
      QMap<SpecializedUnit::SpecializedUnitIdTuple,
           QSharedPointer<SpecializedUnit> > specializedUnits;
      QMap<QString, QSharedPointer<Group> > groups;
    };

    void loadResponseData(LoadedData& loadedData, const DataLoadResponse& resp);
    void loadResponseData(LoadedData& loadedData, const SingleFoodLoadResponse& resp);
    void loadResponseData(LoadedData& loadedData, const CompositeFoodLoadResponse& resp);
    void loadResponseData(LoadedData& loadedData, const TemplateLoadResponse& resp);
    void loadResponseData(LoadedData& loadedData, const MealLoadResponse& resp);
    void loadResponseData(LoadedData& loadedData, const UserLoadResponse& resp);
    void loadResponseData(LoadedData& loadedData, const NutrientLoadResponse& resp);
    void loadResponseData(LoadedData& loadedData, const UnitLoadResponse& resp);
    void loadResponseData(LoadedData& loadedData, const SpecializedUnitLoadResponse& resp);
    void loadResponseData(LoadedData& loadedData, const GroupLoadResponse& resp);

    void setComponents
      (const QSharedPointer<FoodCollectionImpl>& collection_impl,
       const ::google::protobuf::RepeatedPtrField<FoodComponentData>& componentData);
};

template <typename T> void ServiceBackEnd::readResponse(T& t)
{
  // Temporary simple protocol:
  // [type-length] [type-name] [pb-length] [protocol-buffer]
  // Where lengths are 32-bit unsigned integers in network byte order

  quint32 type_len = readResponseLength();

  QByteArray type_bytes;

  while (type_bytes.size() < static_cast<int>(type_len)) {
    if (socket.bytesAvailable() > 0 || socket.waitForReadyRead(3000)) {
      type_bytes += socket.read(type_len - type_bytes.size());
    } else {
      throw std::runtime_error("Socket failed or read timed out");
    }
  }

  std::string respType(type_bytes.data(), type_len);

  if (respType != t.GetTypeName()) {
    throw std::runtime_error("Server responded with unexpected message type");
  }

  quint32 resp_len = readResponseLength();

  QByteArray bytes;

  while (bytes.size() < static_cast<int>(resp_len)) {
    if (socket.bytesAvailable() > 0 || socket.waitForReadyRead(3000)) {
      bytes += socket.read(resp_len - bytes.size());
    } else {
      throw std::runtime_error("Socket failed or read timed out");
    }
  }

  t.ParseFromArray(bytes.data(), resp_len);

  qDebug() << "Received " << QString::fromStdString(respType) << " message:";
  qDebug() << QString::fromStdString(t.DebugString());
}

template <typename T> void ServiceBackEnd::writeMessageAndReadResponse
  (const ::google::protobuf::Message& msg, T& t)
{
  writeMessage(msg);
  readResponse(t);
}

#endif /* SERVICE_BACK_END_H_ */
