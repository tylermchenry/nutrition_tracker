/*
 * meal.cpp
 * Part of nutrition_tracker
 *
 *  Created on: May 22, 2010
 *      Author: Tyler McHenry <tyler@nerdland.net>
 */

#include "meal.h"
#include <QDebug>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlRecord>
#include <QtSql/QSqlField>
#include <QtSql/QSqlError>

QMap<int, QMap<QDate, QMap<int, QWeakPointer<Meal> > > > Meal::mealCache;

QMap<int, QString> Meal::getAllMealNames(int creatorUserId, bool includeGenerics)
{
  QSqlDatabase db = QSqlDatabase::database("nutrition_db");
  QSqlQuery query(db);

  QString queryText = "SELECT meal.Meal_Id, meal.Name FROM meal";

  if (creatorUserId >= 0) {
    queryText += " WHERE meal.CreatorUser_Id=:creatorId";
  }

  if (includeGenerics) {
    if (creatorUserId >= 0) {
      queryText += " OR";
    } else {
      queryText += " WHERE";
    }
    queryText += " meal.CreatorUser_Id IS NULL";
  }

   if (!query.prepare(queryText)) {
     qDebug() << "Query prepare failed: " << query.lastError();
   }

   if (creatorUserId >= 0) {
     query.bindValue(":creatorId", creatorUserId);
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

QSharedPointer<Meal> Meal::getMeal(int userId, const QDate& date, int mealId)
{
  QSqlDatabase db = QSqlDatabase::database("nutrition_db");
  QSqlQuery query(db);

  if (mealCache[userId][date][mealId]) {
    return mealCache[userId][date][mealId].toStrongRef();
  }

  query.prepare("SELECT meal.Meal_Id, meal.CreatorUser_Id, meal.Name, "
                "       meal_link.User_Id, meal_link.MealDate, "
                "       meal_link.Contained_Type, meal_link.Contained_Id, "
                "       meal_link.Magnitude, units.Unit, units.Type, "
                "       units.Name, units.Factor "
                "FROM"
                "        meal "
                "   JOIN meal_link "
                "        ON meal.Meal_Id = meal_link.Meal_Id "
                "   JOIN units "
                "        ON meal_link.Unit = units.Unit "
                "WHERE meal_link.User_Id = :userId "
                "  AND meal_link.MealDate = :date "
                "  AND meal_link.Meal_Id = :mealId ");

  query.bindValue(":userId", userId);
  query.bindValue(":date", date);
  query.bindValue(":mealId", mealId);

  if (query.exec()) {
    qDebug() << "Executed query: " << query.executedQuery();
    return createMealFromQueryResults(query);
  } else {
    return QSharedPointer<Meal>();
  }
}

QVector<QSharedPointer<Meal> > Meal::getMealsForDay(int userId, const QDate& date)
{
  QSqlDatabase db = QSqlDatabase::database("nutrition_db");
  QSqlQuery query(db);

  query.prepare("SELECT meal_link.Meal_Id "
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
  QVector<FoodAmount> components = createComponentsFromQueryResults(query);

  if (query.first()) {

    const QSqlRecord& record = query.record();
    int id = record.field("Meal_Id").value().toInt();
    int userId = record.field("User_Id").value().toInt();
    QDate date = record.field("MealDate").value().toDate();

    if (!mealCache[userId][date][id]) {

      int creatorUserId = -1;

      if (!record.field("CreatorUser_Id").isNull()) {
        creatorUserId = record.field("CreatorUser_Id").value().toInt();
      }

      QSharedPointer<Meal> meal
        (new Meal(id, creatorUserId,
                  record.field("Name").value().toString(),
                  userId, date, components));
      mealCache[userId][date][id] = meal;
      return meal;
    } else {
      return mealCache[userId][date][id].toStrongRef();
    }
  } else {
    return QSharedPointer<Meal>();
  }
}

Meal::~Meal()
{
}

Meal::Meal(int id, int creatorUserId, const QString& name, int userId,
           const QDate& date, const QVector<FoodAmount>& components)
  : CompositeFood("MEAL_" + QString::number(id) + "_" +
                  QString::number(userId) + "_" + date.toString(Qt::ISODate),
                  name, components, 0, 0, 0, 1),
    id(id), creatorUserId(creatorUserId), userId(userId)
{
}

void Meal::saveToDatabase()
{
}

QSharedPointer<Food> Meal::getCanonicalSharedPointer()
{
  return mealCache[userId][date][id].toStrongRef();
}

QSharedPointer<const Food> Meal::getCanonicalSharedPointer() const
{
  return mealCache[userId][date][id].toStrongRef();
}

