/*
 * mysql_back_end.cpp
 *
 *  Created on: Jul 2, 2010
 *      Author: tyler
 */

#include "mysql_back_end.h"
#include "libnutrition/data/data_cache.h"
#include "libnutrition/data/impl/single_food_impl.h"
#include "libnutrition/data/impl/composite_food_impl.h"
#include "libnutrition/data/impl/template_impl.h"
#include "libnutrition/data/impl/meal_impl.h"
#include "libnutrition/data/impl/user_impl.h"
#include "libnutrition/data/impl/nutrient_impl.h"
#include "libnutrition/data/impl/specialized_unit_impl.h"
#include "libnutrition/data/impl/unit_impl.h"
#include "libnutrition/data/impl/group_impl.h"
#include <QVariant>
#include <QDebug>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlRecord>
#include <QtSql/QSqlField>
#include <QtSql/QSqlError>
#include <stdexcept>

MySQLBackEnd::MySQLBackEnd(const QString& db_name)
  : db(QSqlDatabase::database(db_name, true))
{
  if (!db.open()) {
    throw std::runtime_error
      ("Could not open database " + db_name.toStdString() + " as back end.");
  }
}

MySQLBackEnd::~MySQLBackEnd()
{
  db.close();
}

/** Foods **/

QSharedPointer<Food> MySQLBackEnd::loadFood(BackEnd::FoodTypes::FoodType type, int id)
{
  switch (type)
  {
    case BackEnd::FoodTypes::SingleFood:
      return loadSingleFood(id);
      break;

    case BackEnd::FoodTypes::CompositeFood:
      return loadCompositeFood(id);
      break;

    case BackEnd::FoodTypes::Template:
      return loadTemplate(id);
      break;

    default:
      return QSharedPointer<Food>();
  }
}

QMultiMap<QString, QPair<BackEnd::FoodTypes::FoodType, int> >
  MySQLBackEnd::loadFoodNamesForUser(int userId, bool includeExpired)
{
  return loadFoodNamesForUser(userId, BackEnd::FoodTypes::SingleFood, includeExpired) +
    loadFoodNamesForUser(userId, BackEnd::FoodTypes::CompositeFood, includeExpired) +
    loadFoodNamesForUser(userId, BackEnd::FoodTypes::Template, includeExpired);
}


QMultiMap<QString, QPair<BackEnd::FoodTypes::FoodType, int> >
  MySQLBackEnd::loadFoodNamesForUser
    (int userId, BackEnd::FoodTypes::FoodType type, bool includeExpired)
{
  QMultiMap<QString, QPair< BackEnd::FoodTypes::FoodType, int> > foods;
  QMultiMap<QString, int> rawFoods;

  if (type == BackEnd::FoodTypes::SingleFood) {
    rawFoods = loadSingleFoodNamesForUser(userId);
  } else if (type == BackEnd::FoodTypes::CompositeFood) {
    rawFoods = loadCompositeFoodNamesForUser(userId, includeExpired);
  } else if (type == BackEnd::FoodTypes::Template) {
    rawFoods = loadTemplateNamesForUser(userId);
  }

  for (QMultiMap<QString, int>::const_iterator i = rawFoods.begin();
  i != rawFoods.end(); ++i)
  {
    foods.insert(i.key(), qMakePair(type, i.value()));
  }

  return foods;
}

/** Single Foods **/

QSharedPointer<SingleFood>  MySQLBackEnd::loadSingleFood(int id)
{
  QSqlQuery query(db);

  query.prepare("SELECT food_description.Food_Id, food_description.User_Id, "
                "       food_description.Entry_Src, "
                "       food_description.Long_Desc, food_description.Refuse,"
                "       food_description.Ref_desc, food_description.Weight_g, "
                "       food_description.Volume_floz, food_description.Quantity, "
                "       food_description.Servings, food_description.Fat_Factor,"
                "       food_description.CHO_Factor, food_description.Pro_Factor, "
                "       food_description.N_Factor, food_description.FdGrp_Cd, "
                "       nutrient_data.Nutr_No, nutrient_data.Nutr_Val "
                "FROM "
                "       food_description JOIN nutrient_data "
                "           ON food_description.Food_Id = nutrient_data.Food_Id "
                "WHERE "
                "   food_description.Food_Id=:id ");
  query.bindValue(":id", id);

  if (query.exec()) {
    qDebug() << "Executed query: " << query.executedQuery();
    return createSingleFoodFromQueryResults(query);
  } else {
    return QSharedPointer<SingleFood>();
  }
}


QMultiMap<QString, int>  MySQLBackEnd::loadSingleFoodNamesForUser(int userId)
{
  QSqlQuery query(db);
  QMultiMap<QString, int> foods;

  query.prepare("SELECT food_description.Food_Id, food_description.Long_Desc "
                "FROM   food_description "
                "WHERE  food_description.User_Id = :userId");

  query.bindValue(":userId", userId);

  if (query.exec()) {
    while (query.next()) {
      const QSqlRecord& record = query.record();
      foods.insert(record.field("Long_Desc").value().toString(),
                   record.field("Food_Id").value().toInt());
    }
  } else {
    qDebug() << "Query failed: " << query.lastError();
  }

  return foods;
}

void MySQLBackEnd::storeSingleFood(const QSharedPointer<SingleFood>& food)
{
  // TODO: Replace
  food->saveToDatabase();
}


/** Composite Foods **/

QSharedPointer<CompositeFood>  MySQLBackEnd::loadCompositeFood(int id)
{
  QSqlQuery query(db);

  query.prepare("SELECT composite_food.Composite_Id, composite_food.User_Id, "
                "       composite_food.Description, "
                "       composite_food.User_Id, composite_food.IsNonce, "
                "       composite_food.CreationDate, composite_food.ExpiryDate, "
                "       composite_food.Weight_g, composite_food.Volume_floz, "
                "       composite_food.Quantity, composite_food.Servings, "
                "       composite_food_link.CompositeLink_Id, composite_food_link.Contained_Type, "
                "       composite_food_link.Contained_Id, composite_food_link.Includes_Refuse,"
                "       composite_food_link.Magnitude, composite_food_link.Unit, "
                "       composite_food_link.IntrafoodOrder "
                "FROM"
                "        composite_food "
                "   LEFT JOIN composite_food_link "
                "        ON composite_food.Composite_Id = composite_food_link.Composite_Id "
                "WHERE composite_food.Composite_Id = :id "
                "ORDER BY composite_food_link.IntrafoodOrder ASC");

  query.bindValue(":id", id);

  if (query.exec()) {
    qDebug() << "Executed query: " << query.executedQuery();
    return createCompositeFoodFromQueryResults(query);
  } else {
    qDebug() << "Query failed: " << query.lastError();
    return QSharedPointer<CompositeFood>();
  }
}

QMultiMap<QString, int>  MySQLBackEnd::loadCompositeFoodNamesForUser
   (int userId, bool)
{
  QSqlQuery query(db);
  QMultiMap<QString, int> foods;

  // TODO: Use second argument includeExpired

  query.prepare("SELECT composite_food.Composite_Id, composite_food.Description,"
                 "       composite_food.CreationDate, composite_food.ExpiryDate "
                 "FROM   composite_food "
                 "WHERE  composite_food.User_Id = :userId");

  query.bindValue(":userId", userId);

  if (query.exec()) {
    while (query.next()) {
      const QSqlRecord& record = query.record();
      foods.insert(record.field("Description").value().toString() +
                   CompositeFood::generateExpirySuffix
                     (record.field("CreationDate").value().toDate(),
                      record.field("ExpiryDate").value().toDate()),
                   record.field("Composite_Id").value().toInt());
    }
  } else {
    qDebug() << "Query failed: " << query.lastError();
  }

  return foods;
}

void MySQLBackEnd::storeCompositeFood(const QSharedPointer<CompositeFood>& food)
{
  // TODO: Replace
  food->saveToDatabase();
}


/** Templates **/

QSharedPointer<Template> MySQLBackEnd::loadTemplate(int id)
{
  QSqlQuery query(db);

  query.prepare("SELECT template.Template_Id, template.User_Id, template.Description, "
                "       template_link.TemplateLink_Id, template_link.Contained_Type, "
                "       template_link.Contained_Id, template_link.Includes_Refuse, "
                "       template_link.Magnitude, template_link.Unit, "
                "       template_link.IntrafoodOrder "
                "FROM"
                "        template "
                "   LEFT JOIN template_link "
                "        ON template.Template_Id = template_link.Template_Id "
                "WHERE template.Template_Id = :id "
                "ORDER BY template_link.IntrafoodOrder ASC");

  query.bindValue(":id", id);

  if (query.exec()) {
    qDebug() << "Executed query: " << query.executedQuery();
    return createTemplateFromQueryResults(query);
  } else {
    qDebug() << "Query failed: " << query.lastError();
    return QSharedPointer<Template>();
  }
}

QMultiMap<QString, int> MySQLBackEnd::loadTemplateNamesForUser(int userId)
{
  QSqlQuery query(db);
  QMultiMap<QString, int> foods;

  query.prepare("SELECT template.Template_Id, template.Description "
                 "FROM   template "
                 "WHERE  template.User_Id = :userId");

  query.bindValue(":userId", userId);

  if (query.exec()) {
    while (query.next()) {
      const QSqlRecord& record = query.record();
      foods.insert(record.field("Description").value().toString(),
                   record.field("Template_Id").value().toInt());
    }
  } else {
    qDebug() << "Query failed: " << query.lastError();
  }

  return foods;
}

void MySQLBackEnd::storeTemplate(const QSharedPointer<Template>& templ)
{
  // TODO: Replace
  templ->saveToDatabase();
}


 /** Meals **/

QSharedPointer<Meal> MySQLBackEnd::loadMeal
   (int userId, const QDate& date, int mealId)
{
  QSqlQuery query(db);

  query.prepare("SELECT meal.Meal_Id, meal.CreatorUser_Id, meal.Name, "
                "       meal_link.MealLink_Id, meal_link.User_Id, meal_link.MealDate, "
                "       meal_link.Contained_Type, meal_link.Contained_Id, "
                "       meal_link.Includes_Refuse, meal_link.Magnitude, "
                "       meal_link.Unit, meal_link.IntramealOrder "
                "FROM"
                "        meal "
                "   JOIN meal_link "
                "        ON meal.Meal_Id = meal_link.Meal_Id "
                "WHERE meal_link.User_Id = :userId "
                "  AND meal_link.MealDate = :date "
                "  AND meal_link.Meal_Id = :mealId "
                "ORDER BY IntramealOrder ASC");

  query.bindValue(":userId", userId);
  query.bindValue(":date", date);
  query.bindValue(":mealId", mealId);

  if (query.exec()) {
    qDebug() << "Executed query: " << query.executedQuery();
    qDebug() << "There are " << query.size() << " foods in this meal";
    return createMealFromQueryResults(query);
  } else {
    qDebug() << "Query failed: " << query.lastError();
    return QSharedPointer<Meal>();
  }
}

QMap<int, QString> MySQLBackEnd::loadAllMealNames()
{
  return loadAllMealNamesForUser(-1, true);
}

QMap<int, QString> MySQLBackEnd::loadAllMealNamesForUser
   (int userId, bool includeGenerics)
{
  QSqlQuery query(db);

  QString queryText = "SELECT meal.Meal_Id, meal.Name FROM meal";

  if (userId >= 0) {
    queryText += " WHERE meal.CreatorUser_Id=:userId";
  }

  if (includeGenerics) {
    if (userId >= 0) {
      queryText += " OR";
    } else {
      queryText += " WHERE";
    }
    queryText += " meal.CreatorUser_Id IS NULL";
  }

   if (!query.prepare(queryText)) {
     qDebug() << "Query prepare failed: " << query.lastError();
   }

   if (userId >= 0) {
     query.bindValue(":userId", userId);
   }

   QMap<int, QString> mealNames;

   if (query.exec()) {
     qDebug() << "Executed query: " << query.executedQuery();
     while (query.next()) {
       mealNames[query.record().field("Meal_Id").value().toInt()] =
         query.record().field("Name").value().toString();
     }
   } else {
     qDebug() << "Query failed: " << query.lastError();
   }

   return mealNames;
}

QList<QSharedPointer<Meal> > MySQLBackEnd::loadAllMealsForDay
   (int userId, const QDate& date)
{
  QSqlQuery query(db);

  query.prepare("SELECT DISTINCT meal_link.Meal_Id "
                "FROM meal_link "
                "WHERE meal_link.User_Id = :userId "
                "  AND meal_link.MealDate = :date ");

  query.bindValue(":userId", userId);
  query.bindValue(":date", date);

  QList<QSharedPointer<Meal> > meals;

  if (query.exec()) {
    qDebug() << "Executed query: " << query.executedQuery();
    while (query.next()) {
      meals.push_back
        (Meal::getMeal
          (userId, date, query.record().field("Meal_Id").value().toInt()));
    }
  }

  return meals;
}

void MySQLBackEnd::storeMeal(const QSharedPointer<Meal>& meal)
{
  // TODO: Replace
  meal->saveToDatabase();
}


/** Users **/

QSharedPointer<User> MySQLBackEnd::loadUser(int id)
{
  QSqlQuery query(db);

  query.prepare("SELECT User_Id, User_Name, Name, PW_SHA1 FROM user "
                 " WHERE User_Id=:id LIMIT 1");
  query.bindValue(":id", id);

  if (query.exec() && query.first()) {
    return createUserFromRecord(query.record());
  } else {
    return QSharedPointer<User>();
  }
}

QSharedPointer<User> MySQLBackEnd::loadUserByUsername
  (const QString& username)
{
  QSqlQuery query(db);

  query.prepare("SELECT User_Id, User_Name, Name, PW_SHA1 FROM user "
                 " WHERE User_Name=:username LIMIT 1");
  query.bindValue(":username", username);

  if (query.exec() && query.first()) {
    return createUserFromRecord(query.record());
  } else {
    return QSharedPointer<User>();
  }
}

QList<QSharedPointer<User> > MySQLBackEnd::loadAllUsers()
{
  QSqlDatabase db = QSqlDatabase::database("nutrition_db");
  QSqlQuery query(db);

  query.prepare("SELECT User_Id, User_Name, Name, PW_SHA1 FROM user "
                 " ORDER BY User_Name ASC");

  if (query.exec()) {
    return createUsersFromQueryResults(query);
  } else {
    return QList<QSharedPointer<User> >();
  }
}

void MySQLBackEnd::storeUser(const QSharedPointer<User>& user)
{
  // TODO: Replace
  user->saveToDatabase();
}


/** Nutrients **/

QSharedPointer<const Nutrient> MySQLBackEnd::loadNutrient(const QString& id)
{
  QSqlQuery query(db);

  query.prepare("SELECT nutrient_definition.Nutr_No, nutrient_definition.Category, "
                "  nutrient_definition.ShortName, nutrient_definition.RDI, "
                "  units.Unit, units.Type, units.Name AS UnitName, units.Factor "
                "FROM nutrient_definition JOIN units ON nutrient_definition.Units = units.Unit "
                "WHERE Nutr_No=:id "
                "ORDER BY nutrient_definition.category ASC, nutrient_definition.Disp_Order ASC, "
                "  nutrient_definition.ShortName ASC "
                "LIMIT 1");
  query.bindValue(":id", id);

  if (query.exec() && query.first()) {
    return createNutrientFromRecord(query.record());
  } else {
    return QSharedPointer<Nutrient>();
  }
}

QList<QSharedPointer<Nutrient> > MySQLBackEnd::loadAllNutrients()
{
  QSqlQuery query(db);

  query.prepare("SELECT nutrient_definition.Nutr_No, nutrient_definition.Category, "
                "  nutrient_definition.ShortName, nutrient_definition.RDI, "
                "  units.Unit, units.Type, units.Name AS UnitName, units.Factor "
                "FROM nutrient_definition JOIN units ON nutrient_definition.Units = units.Unit "
                "ORDER BY nutrient_definition.category ASC, nutrient_definition.Disp_Order ASC, "
                "  nutrient_definition.ShortName ASC");

  QList<QSharedPointer<Nutrient> > nutrients;

  if (query.exec()) {
    nutrients = createNutrientsFromQueryResults(query);
  }

  return nutrients;
}

QList<QSharedPointer<Nutrient> > MySQLBackEnd::loadAllNutrients
  (Nutrient::Categories::Category category)
{
  QSqlQuery query(db);

  query.prepare("SELECT nutrient_definition.Nutr_No, nutrient_definition.Category, "
                "  nutrient_definition.ShortName, nutrient_definition.RDI, "
                "  units.Unit, units.Type, units.Name AS UnitName, units.Factor "
                "FROM nutrient_definition JOIN units ON nutrient_definition.Units = units.Unit "
                "WHERE nutrient_definition.Category=:category "
                "ORDER BY nutrient_definition.category ASC, nutrient_definition.Disp_Order ASC, "
                "  nutrient_definition.ShortName ASC");

  query.bindValue(":category", Nutrient::Categories::toHumanReadable(category));

  QList<QSharedPointer<Nutrient> > nutrients;

  if (query.exec()) {
    nutrients = createNutrientsFromQueryResults(query);
  }

  return nutrients;
}


/** Units **/

QSharedPointer<const Unit> MySQLBackEnd::loadUnit(const QString& abbr)
{
  QSqlQuery query(db);

  query.prepare("SELECT Unit, Type, Name AS UnitName, Factor FROM units WHERE Unit=:abbrev "
                "ORDER BY Name LIMIT 1");
  query.bindValue(":abbrev", abbr);

  if (query.exec() && query.first()) {
    return createUnitFromRecord(query.record());
  } else {
    return QSharedPointer<Unit>();
  }
}

QList<QSharedPointer<Unit> > MySQLBackEnd::loadAllUnits()
{
  QSqlQuery query(db);

  if (query.exec("SELECT Unit, Type, Name AS UnitName, Factor FROM units ORDER BY Name")) {
    return createUnitsFromQueryResults(query);
  } else {
    return QList<QSharedPointer<Unit> >();
  }
}

QList<QSharedPointer<Unit> > MySQLBackEnd::loadAllUnits
  (Unit::Dimensions::Dimension dimension)
{
  QSqlQuery query(db);

  query.prepare("SELECT Unit, Type, Name AS UnitName, Factor FROM units WHERE Type=:dim ORDER BY Name");
  query.bindValue(":dim", Unit::Dimensions::toHumanReadable(dimension));

  if (query.exec()) {
    return createUnitsFromQueryResults(query);
  } else {
    return QList<QSharedPointer<Unit> >();
  }
}

QSharedPointer<const Unit> MySQLBackEnd::loadBasicUnit
  (Unit::Dimensions::Dimension dimension)
{
  QSqlQuery query(db);

  query.prepare("SELECT Unit, Type, Name AS UnitName, Factor FROM units WHERE Type=:dim AND Factor=1 "
                "ORDER BY Name LIMIT 1");
  query.bindValue(":dim", Unit::Dimensions::toHumanReadable(dimension));

  QList<QSharedPointer<Unit> > units;

  if (query.exec()) {
    units = createUnitsFromQueryResults(query);
  }

  if (units.size() > 0) {
    return units[0];
  } else {
    return QSharedPointer<const Unit>();
  }
}

QSharedPointer<const SpecializedUnit>
  MySQLBackEnd::loadSpecializedUnit(int foodId, int sequence)
{
  QSqlQuery query(db);

  query.prepare("SELECT Food_Id, Seq, Amount, Msre_Desc, Gm_Wgt "
                " FROM weight WHERE Food_Id=:foodId AND Seq=:sequence "
                " ORDER BY Seq LIMIT 1");
  query.bindValue(":foodId", foodId);
  query.bindValue(":sequence", sequence);

  if (query.exec() && query.first()) {
    return createSpecializedUnitFromRecord(query.record());
  } else {
    return QSharedPointer<SpecializedUnit>();
  }
}

QList<QSharedPointer<SpecializedUnit> >
  MySQLBackEnd::loadAllSpecializedUnitsForFood(int foodId)
{
  QSqlQuery query(db);

  query.prepare("SELECT Food_Id, Seq, Amount, Msre_Desc, Gm_Wgt "
                 " FROM weight WHERE Food_Id=:foodId ORDER BY Seq");
  query.bindValue(":foodId", foodId);

  if (query.exec()) {
    return createSpecializedUnitsFromQueryResults(query);
  } else {
    return QList<QSharedPointer<SpecializedUnit> >();
  }
}

QPair<QList<QSharedPointer<Unit> >,
      QList<QSharedPointer<SpecializedUnit> > >
  MySQLBackEnd::loadAllUnitsForFood(int foodId)
{
  return qMakePair(loadAllUnits(), loadAllSpecializedUnitsForFood(foodId));
}


/** Groups **/

QSharedPointer<const Group> MySQLBackEnd::loadGroup(const QString& id)
{
  QSqlQuery query(db);

  query.prepare("SELECT FdGrp_Cd, FdGrp_Desc FROM group_description WHERE FdGrp_cd=:id "
                "ORDER BY FdGrp_Desc ASC LIMIT 1");
  query.bindValue(":id", id);

  if (query.exec() && query.first()) {
    return createGroupFromRecord(query.record());
  } else {
    return QSharedPointer<Group>();
  }
}

QList<QSharedPointer<Group> > MySQLBackEnd::loadAllGroups()
{
  QSqlQuery query(db);

  if (query.exec("SELECT FdGrp_Cd, FdGrp_Desc FROM group_description "
                 "ORDER BY FdGrp_Desc ASC")) {
    return createGroupsFromQueryResults(query);
  } else {
    return QList<QSharedPointer<Group> >();
  }
}

/** Log In / Register **/

QSharedPointer<User> MySQLBackEnd::logIn
  (const QString& username, const QString& password, QString& errorMessage)
{
  QSharedPointer<User> user = User::getUserByUsername(username);

  if (!user) {
    errorMessage = "Invalid user object";
  } else if (!user->checkPassword(password)){
    errorMessage = "Incorrect password";
  } else {
    return user;
  }

  return QSharedPointer<User>();
}

QSharedPointer<User>
  MySQLBackEnd::registerAndLogIn
    (const QString& username, const QString& realName,
     const QString& password, QString& errorMessage)
{
  if (User::getUserByUsername(username)) {
    errorMessage = "Username is already in use";
    return QSharedPointer<User>();
  } else if (password.isEmpty()) {
    errorMessage = "Password cannot be blank";
    return QSharedPointer<User>();
  }

  QSqlQuery query(db);

  query.prepare("INSERT INTO user (User_Id, User_Name, Name, PW_SHA1) "
                " VALUES (NULL, :username, :realName, "
                "         SHA1(CONCAT(:password, :username2)))");
  query.bindValue(":username", username);
  query.bindValue(":username2", username);
  query.bindValue(":realName", realName);
  query.bindValue(":password", password);

  if (query.exec()) {
    QSharedPointer<User> user = User::getUser(query.lastInsertId().toInt());
    if (!user) {
      errorMessage = "Unknown failure";
    }
    return user;
  } else {
    errorMessage = "SQL Failure: " + query.lastError().text();
    return QSharedPointer<User>();
  }
}


/** Search **/

QList<MySQLBackEnd::SearchResult> MySQLBackEnd::searchFoods
  (const SearchRequest&)
{
  return QList<SearchResult>();
}


/** Object Construction (private) **/

QSharedPointer<SingleFood> MySQLBackEnd::createSingleFoodFromQueryResults
  (QSqlQuery& query)
{
  QMap<QString, NutrientAmount> nutrients;

   while (query.next()) {
     QSharedPointer<const Nutrient> nutrient =
       Nutrient::getNutrient(query.record().field("Nutr_No").value().toString());

     if (nutrient != NULL) {
       nutrients[nutrient->getId()] = NutrientAmount
         (nutrient, query.record().field("Nutr_Val").value().toDouble());
     }
   }

   if (query.first()) {
     const QSqlRecord& record = query.record();
     int id = record.field("Food_Id").value().toInt();
     QSharedPointer<SingleFood> food
       (new SingleFoodImpl(id,
                           record.field("Long_Desc").value().toString(),
                           record.field("User_Id").value().toInt(),
                           SingleFood::EntrySources::fromHumanReadable
                           (record.field("Entry_Src").value().toString()),
                           Group::getGroup(record.field("FdGrp_Cd").value().toString()),
                           record.field("Refuse").value().toDouble(),
                           record.field("Ref_desc").value().toString(),
                           nutrients,
                           record.field("Weight_g").value().toDouble(),
                           record.field("Volume_floz").value().toDouble(),
                           record.field("Quantity").value().toDouble(),
                           record.field("Servings").value().toDouble(),
                           record.field("Fat_Factor").value().toDouble(),
                           record.field("CHO_Factor").value().toDouble(),
                           record.field("Pro_Factor").value().toDouble(),
                           record.field("N_Factor").value().toDouble()));
     DataCache<SingleFood>::getInstance().insert(id, food);
     return food;
   } else {
     return QSharedPointer<SingleFood>();
   }
}

QSharedPointer<CompositeFood>
  MySQLBackEnd::createCompositeFoodFromQueryResults
    (QSqlQuery& query)
{
  QSharedPointer<CompositeFoodImpl> food_impl;

  if (query.first()) {

    const QSqlRecord& record = query.record();
    int id = record.field("Composite_Id").value().toInt();

    food_impl = QSharedPointer<CompositeFoodImpl>
      (new CompositeFoodImpl(id,
                             record.field("Description").value().toString(),
                             record.field("User_Id").value().toInt(),
                             record.field("Weight_g").value().toDouble(),
                             record.field("Volume_floz").value().toDouble(),
                             record.field("Quantity").value().toDouble(),
                             record.field("Servings").value().toDouble(),
                             record.field("CreationDate").value().toDate(),
                             record.field("ExpiryDate").value().toDate(),
                             record.field("IsNonce").value().toBool()));

    DataCache<CompositeFood>::getInstance().insert(id, food_impl);
  }

  if (food_impl && query.isActive()) {
    query.seek(-1); // Reset to before first record
    food_impl->setComponents
      (createComponentsFromQueryResults
         (query, food_impl, "CompositeLink_Id", "IntrafoodOrder"));
    if (food_impl->needsToBeReSaved()) {
      food_impl->saveToDatabase();
    }
  }

  return food_impl;
}

QSharedPointer<Template> MySQLBackEnd::createTemplateFromQueryResults
  (QSqlQuery& query)
{
  QSharedPointer<TemplateImpl> food_impl;

  if (query.first()) {

    const QSqlRecord& record = query.record();
    int id = record.field("Template_Id").value().toInt();

    food_impl = QSharedPointer<TemplateImpl>
    (new TemplateImpl(id,
                      record.field("Description").value().toString(),
                      record.field("User_Id").value().toInt()));
    DataCache<Template>::getInstance().insert(id, food_impl);
  }

  if (food_impl && query.isActive()) {
    query.seek(-1); // Reset to before first record
    food_impl->setComponents
      (createComponentsFromQueryResults
         (query, food_impl, "TemplateLink_Id", "IntrafoodOrder"));
    if (food_impl->needsToBeReSaved()) {
      food_impl->saveToDatabase();
    }
  }

  return food_impl;
}

QSharedPointer<Meal> MySQLBackEnd::createMealFromQueryResults
  (QSqlQuery& query)
{
  QSharedPointer<MealImpl> meal_impl;

  if (query.first()) {

    const QSqlRecord& record = query.record();
    int id = record.field("Meal_Id").value().toInt();
    int userId = record.field("User_Id").value().toInt();
    QDate date = record.field("MealDate").value().toDate();

    int creatorId = -1;

    if (!record.field("CreatorUser_Id").isNull()) {
      creatorId = record.field("CreatorUser_Id").value().toInt();
    }

    meal_impl = QSharedPointer<MealImpl>
      (new MealImpl(id, creatorId,
                    record.field("Name").value().toString(),
                    userId, date));

    DataCache<Meal>::getInstance().insert(meal_impl->getMealIdTuple(), meal_impl);
  }

  if (meal_impl && query.isActive()) {
    query.seek(-1); // Reset to before first record
    meal_impl->setComponents
    (createComponentsFromQueryResults(query, meal_impl, "MealLink_Id", "Intrameal_implOrder"));
    if (meal_impl->needsToBeReSaved()) {
      meal_impl->saveToDatabase();
    }
  } else if (!meal_impl) {
    qDebug() << "Meal was not created!";
  } else {
    qDebug() << "Could not rewind query!";
  }

  return meal_impl;
}

QList<QSharedPointer<User> > MySQLBackEnd::createUsersFromQueryResults
  (QSqlQuery& query)
{
  QList<QSharedPointer<User> > users;

  while (query.next()) {
    users.push_back(createUserFromRecord(query.record()));
  }

  return users;
}

QSharedPointer<User> MySQLBackEnd::createUserFromRecord
  (const QSqlRecord& record)
{
  if (!record.isEmpty()) {
    int id = record.field("User_Id").value().toInt();
    QSharedPointer<User> user
    (new UserImpl(id,
                  record.field("User_Name").value().toString(),
                  record.field("Name").value().toString(),
                  record.field("PW_SHA1").value().toString()));
    DataCache<User>::getInstance().insert(id, user);
    return user;
  } else {
    return QSharedPointer<User>();
  }
}

QList<QSharedPointer<Nutrient> > MySQLBackEnd::createNutrientsFromQueryResults
  (QSqlQuery& query)
{
  QList<QSharedPointer<Nutrient> > nutrients;

  while (query.next()) {
    nutrients.push_back(createNutrientFromRecord(query.record()));
  }

  return nutrients;
}

QSharedPointer<Nutrient> MySQLBackEnd::createNutrientFromRecord
  (const QSqlRecord& record)
{
  if (!record.isEmpty()) {
    QString id = record.field("Nutr_No").value().toString();
    QSharedPointer<Nutrient> nutrient
      (new NutrientImpl(id,
                        record.field("ShortName").value().toString(),
                        Nutrient::Categories::fromHumanReadable
                        (record.field("Category").value().toString()),
                        createUnitFromRecord(record),
                        record.field("RDI").value().toDouble()));
    qDebug() << "Added nutrient named " << nutrient->getName() << " to cache at ID " << id;
    DataCache<Nutrient>::getInstance().insert(id, nutrient);
    return nutrient;
  } else {
    return QSharedPointer<Nutrient>();
  }
}

QList<QSharedPointer<Unit> > MySQLBackEnd::createUnitsFromQueryResults
  (QSqlQuery& query)
{
  QList<QSharedPointer<Unit> > units;

  while (query.next()) {
    units.push_back(createUnitFromRecord(query.record()));
  }

  return units;
}

QSharedPointer<Unit> MySQLBackEnd::createUnitFromRecord
  (const QSqlRecord& record)
{
  if (!record.isEmpty()) {
    QString abbrev = record.field("Unit").value().toString();
    QSharedPointer<Unit> unit
      (new UnitImpl(abbrev,
                    record.field("UnitName").value().toString(),
                    Unit::Dimensions::fromHumanReadable
                      (record.field("Type").value().toString()),
                    record.field("Factor").value().toDouble()));
    DataCache<Unit>::getInstance().insert(abbrev, unit);
    return unit;
  } else {
    return QSharedPointer<Unit>();
  }
}

QList<QSharedPointer<SpecializedUnit> >
  MySQLBackEnd::createSpecializedUnitsFromQueryResults
    (QSqlQuery& query)
{
  QList<QSharedPointer<SpecializedUnit> > spUnits;

  while (query.next()) {
    spUnits.push_back(createSpecializedUnitFromRecord(query.record()));
  }

  return spUnits;
}

QSharedPointer<SpecializedUnit> MySQLBackEnd::createSpecializedUnitFromRecord
  (const QSqlRecord& record)
{
  if (!record.isEmpty()) {
    int foodId = record.field("Food_Id").value().toInt();
    int sequence = record.field("Seq").value().toInt();
    SpecializedUnit::SpecializedUnitIdTuple idTuple(foodId, sequence);
    QSharedPointer<SpecializedUnit> spUnit
      (new SpecializedUnitImpl(foodId, sequence,
                               record.field("Msre_Desc").value().toString(),
                               record.field("Gm_Wgt").value().toDouble()));
    DataCache<SpecializedUnit>::getInstance().insert(idTuple, spUnit);
    return spUnit;
  } else {
    return QSharedPointer<SpecializedUnit>();
  }
}

QList<QSharedPointer<Group> > MySQLBackEnd::createGroupsFromQueryResults
  (QSqlQuery& query)
{
  QList<QSharedPointer<Group> > groups;

  while (query.next()) {
    groups.push_back(createGroupFromRecord(query.record()));
  }

  return groups;
}

QSharedPointer<Group> MySQLBackEnd::createGroupFromRecord
  (const QSqlRecord& record)
{
  if (!record.isEmpty()) {
    QString id = record.field("FdGrp_Cd").value().toString();
    QSharedPointer<Group> group
      (new GroupImpl(id, record.field("FdGrp_Desc").value().toString()));
    DataCache<Group>::getInstance().insert(id, group);
    return group;
  } else {
    return QSharedPointer<Group>();
  }
}

QList<FoodComponent> MySQLBackEnd::createComponentsFromQueryResults
  (QSqlQuery& query, const QSharedPointer<FoodCollection>& containingCollection,
   const QString& componentIdField, const QString& componentOrderField)
{
  QList<FoodComponent> components;
  int order = -1;

  while (query.next()) {

    const QSqlRecord& record = query.record();

    if (record.field(componentIdField).value().isNull()) continue;

    int containedId = record.field("Contained_Id").value().toInt();

    FoodCollection::ContainedTypes::ContainedType type =
      FoodCollection::ContainedTypes::fromHumanReadable(record.field("Contained_Type").value().toString());

    QSharedPointer<Food> containedFood;

    if (type == FoodCollection::ContainedTypes::SingleFood) {
      containedFood = SingleFood::getSingleFood(containedId);
    } else if (type == FoodCollection::ContainedTypes::CompositeFood) {
      containedFood = CompositeFood::getCompositeFood(containedId);
    } else {
      qDebug() << "Component has unknown type!";
    }

    if (!record.field(componentOrderField).value().isNull()) {
      order = record.field(componentOrderField).value().toInt();
    } else {
      qDebug() << "WARNING: Order field " << componentOrderField
               << " missing. Defaulting to sequential.";
      order++;
    }

    if (containedFood != NULL) {
      qDebug() << "Value of component ID field " << componentIdField << ": "
                << record.field(componentIdField).value().toInt();
      components.append
        (FoodComponent
          (containingCollection,
           record.field(componentIdField).value().toInt(),
           FoodAmount(containedFood, record.field("Magnitude").value().toDouble(),
                      Unit::getUnit(record.field("Unit").value().toString()),
                      record.field("Includes_Refuse").value().toBool()),
           order));
    } else {
      qDebug() << "Failed to create component!";
    }
  }

  return components;
}
