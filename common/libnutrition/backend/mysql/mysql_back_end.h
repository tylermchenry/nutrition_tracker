/*
 * mysql_back_end.h
 *
 *  Created on: Jul 2, 2010
 *      Author: tyler
 */

#ifndef MYSQL_BACK_END_H_
#define MYSQL_BACK_END_H_

#include "../back_end.h"
#include <QString>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>

class MySQLBackEnd : public BackEnd
{
  public:

    using BackEnd::FoodTypes;
    using BackEnd::SearchRequest;
    using BackEnd::SearchResult;

    MySQLBackEnd(const QString& db_name);

    virtual ~MySQLBackEnd();

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
      logInWithPasswordHash
        (const QString& username, const QString& password_hash,
        QString& errorMessage);

    virtual QSharedPointer<User>
      registerAndLogIn(const QString& username, const QString& realName,
                         const QString& password, QString& errorMessage);

    virtual QSharedPointer<User>
      registerAndLogInWithPasswordHash
        (const QString& username, const QString& realName,
         const QString& password_hash, QString& errorMessage);

    /** Search **/

    virtual QList<SearchResult> searchFoods(const SearchRequest& request);


  private:

    QSqlDatabase db;

    QSharedPointer<SingleFood> createSingleFoodFromQueryResults
      (QSqlQuery& query);

    QSharedPointer<CompositeFood> createCompositeFoodFromQueryResults
      (QSqlQuery& query);

    QSharedPointer<Template> createTemplateFromQueryResults
      (QSqlQuery& query);

    QSharedPointer<Meal> createMealFromQueryResults
      (QSqlQuery& query);

    QList<QSharedPointer<User> > createUsersFromQueryResults
      (QSqlQuery& query);

    QSharedPointer<User> createUserFromRecord
      (const QSqlRecord& record);

    QList<QSharedPointer<Nutrient> > createNutrientsFromQueryResults
      (QSqlQuery& query);

    QSharedPointer<Nutrient> createNutrientFromRecord
      (const QSqlRecord& record);

    QList<QSharedPointer<Unit> > createUnitsFromQueryResults
      (QSqlQuery& query);

    QSharedPointer<const Unit> createUnitFromRecordOrCache
      (const QSqlRecord& record);

    QSharedPointer<Unit> createUnitFromRecord
      (const QSqlRecord& record);

    QList<QSharedPointer<SpecializedUnit> >
      createSpecializedUnitsFromQueryResults
        (QSqlQuery& query);

    QSharedPointer<SpecializedUnit> createSpecializedUnitFromRecord
      (const QSqlRecord& record);

    QList<QSharedPointer<Group> > createGroupsFromQueryResults
      (QSqlQuery& query);

    QSharedPointer<Group> createGroupFromRecord
      (const QSqlRecord& record);

    QList<FoodComponent> createComponentsFromQueryResults
      (QSqlQuery& query,
       const QSharedPointer<FoodCollection>& containingCollection,
       const QString& componentIdField,
       const QString& componentOrderField = "Order") const;

    void bindBaseAmount
      (const QSharedPointer<Food>& food, QSqlQuery& query,
       const QString& placeholder, Unit::Dimensions::Dimension dimension) const;

    void runSearchQuery
      (const QString& queryText, const QString& terms,
       QMap<QString, SearchResult>& results) const;
};

#endif /* MYSQL_BACK_END_H_ */
