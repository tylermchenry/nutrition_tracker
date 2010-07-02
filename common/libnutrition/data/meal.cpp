/*
 * meal.cpp
 * Part of nutrition_tracker
 *
 *  Created on: May 22, 2010
 *      Author: Tyler McHenry <tyler@nerdland.net>
 */

#include "meal.h"
#include "impl/meal_impl.h"
#include "data_cache.h"
#include "single_food.h"
#include "composite_food.h"
#include <QDebug>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlRecord>
#include <QtSql/QSqlField>
#include <QtSql/QSqlError>

int Meal::nextTemporaryId = 0;

QMap<int, QString> Meal::getAllMealNames(int creatorId, bool includeGenerics)
{
  QSqlDatabase db = QSqlDatabase::database("nutrition_db");
  QSqlQuery query(db);

  QString queryText = "SELECT meal.Meal_Id, meal.Name FROM meal";

  if (creatorId >= 0) {
    queryText += " WHERE meal.CreatorUser_Id=:creatorId";
  }

  if (includeGenerics) {
    if (creatorId >= 0) {
      queryText += " OR";
    } else {
      queryText += " WHERE";
    }
    queryText += " meal.CreatorUser_Id IS NULL";
  }

   if (!query.prepare(queryText)) {
     qDebug() << "Query prepare failed: " << query.lastError();
   }

   if (creatorId >= 0) {
     query.bindValue(":creatorId", creatorId);
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

QSharedPointer<Meal> Meal::createTemporaryMeal(int userId, const QDate& date, int mealId)
{
  QSharedPointer<Meal> meal
    (new MealImpl(mealId, userId,
                  getAllMealNames()[mealId],
                  userId, date, QList<FoodComponent>(), nextTemporaryId++));

  DataCache<TemporaryMeal>::getInstance().insert(meal->getTemporaryId(), meal);
  return meal;
}

QSharedPointer<Meal> Meal::getOrCreateMeal(int userId, const QDate& date, int mealId)
{
  QSharedPointer<Meal> meal = getMeal(userId, date, mealId);

  if (meal == NULL) {
    QSharedPointer<Meal> newMeal
      (new MealImpl(mealId, userId,
                    getAllMealNames()[mealId],
                    userId, date, QList<FoodComponent>()));
    DataCache<Meal>::getInstance().insert(newMeal->getMealIdTuple(), newMeal);
    meal = newMeal;
  }

  return meal;
}

QSharedPointer<Meal> Meal::getMeal(int userId, const QDate& date, int mealId)
{
  QSqlDatabase db = QSqlDatabase::database("nutrition_db");
  QSqlQuery query(db);

  if (DataCache<Meal>::getInstance().contains(MealIdTuple(userId, date, mealId)))
  {
    DataCache<Meal>::getInstance().get(MealIdTuple(userId, date, mealId));
  }

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

QVector<QSharedPointer<Meal> > Meal::getMealsForDay(int userId, const QDate& date)
{
  QSqlDatabase db = QSqlDatabase::database("nutrition_db");
  QSqlQuery query(db);

  query.prepare("SELECT DISTINCT meal_link.Meal_Id "
                "FROM meal_link "
                "WHERE meal_link.User_Id = :userId "
                "  AND meal_link.MealDate = :date ");

  query.bindValue(":userId", userId);
  query.bindValue(":date", date);

  QVector<QSharedPointer<Meal> > meals;

  if (query.exec()) {
    qDebug() << "Executed query: " << query.executedQuery();
    while (query.next()) {
      meals.push_back(getMeal(userId, date, query.record().field("Meal_Id").value().toInt()));
    }
  }

  return meals;
}

QSharedPointer<Meal> Meal::createMealFromQueryResults(QSqlQuery& query)
{
  QSharedPointer<Meal> meal;

  if (query.first()) {

    const QSqlRecord& record = query.record();
    int id = record.field("Meal_Id").value().toInt();
    int userId = record.field("User_Id").value().toInt();
    QDate date = record.field("MealDate").value().toDate();

    if (!DataCache<Meal>::getInstance().contains(MealIdTuple(userId, date, id))) {

      int creatorId = -1;

      if (!record.field("CreatorUser_Id").isNull()) {
        creatorId = record.field("CreatorUser_Id").value().toInt();
      }

      meal = QSharedPointer<Meal>
        (new MealImpl(id, creatorId,
                      record.field("Name").value().toString(),
                      userId, date));

      DataCache<Meal>::getInstance().insert(meal->getMealIdTuple(), meal);

    } else {
      return DataCache<Meal>::getInstance().get(MealIdTuple(userId, date, id));
    }
  }

  if (meal && query.isActive()) {
    query.seek(-1); // Reset to before first record
    meal->setComponents
      (createComponentsFromQueryResults(query, meal, "MealLink_Id", "IntramealOrder"));
    if (meal->needsToBeReSaved()) {
      meal->saveToDatabase();
    }
  } else if (!meal) {
    qDebug() << "Meal was not created!";
  } else {
    qDebug() << "Could not rewind query!";
  }

  return meal;
}

QSharedPointer<Food> Meal::getCanonicalSharedPointer() const
{
  if (isTemporary()) {
    return DataCache<TemporaryMeal>::getInstance().get(getTemporaryId());
  } else {
    return DataCache<Meal>::getInstance().get(getMealIdTuple());
  }
}

Meal::MealIdTuple::MealIdTuple(int uid, const QDate& d, int mid)
  : userId(uid), date(d), mealId(mid)
{
}

bool Meal::MealIdTuple::operator< (const MealIdTuple& rhs) const
{
  return userId < rhs.userId ||
    (userId == rhs.userId && date < rhs.date) ||
    (userId == rhs.userId && date == rhs.date && mealId < rhs.mealId);
}


