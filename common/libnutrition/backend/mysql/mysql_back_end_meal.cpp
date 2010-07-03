#include "mysql_back_end.h"
#include "libnutrition/data/data_cache.h"
#include "libnutrition/data/impl/meal_impl.h"
#include <QVariant>
#include <QDebug>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlRecord>
#include <QtSql/QSqlField>
#include <QtSql/QSqlError>
#include <stdexcept>

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
