/*
 * meal.h
 * Part of nutrition_tracker
 *
 *  Created on: May 22, 2010
 *      Author: Tyler McHenry <tyler@nerdland.net>
 */

#ifndef MEAL_H_
#define MEAL_H_

#include "food_collection.h"
#include <QDate>
#include <QMap>

class Meal : public FoodCollection
{
  public:

    static QMap<int, QString> getAllMealNames
      (int creatorUserId = -1, bool includeGenerics = true);

    static QSharedPointer<Meal> createTemporaryMeal(int userId, const QDate& date, int mealId);

    static QSharedPointer<Meal> getOrCreateMeal(int userId, const QDate& date, int mealId);

    static QSharedPointer<Meal> getMeal(int userId, const QDate& date, int mealId);

    static QVector<QSharedPointer<Meal> > getMealsForDay(int userId, const QDate& date);

    static QSharedPointer<Meal> createMealFromQueryResults(QSqlQuery& query);

    virtual ~Meal();

    inline int getMealId() const { return id; }

    inline QDate getDate() const { return date; }

    virtual void mergeMeal(const QSharedPointer<const Meal>& meal);

    virtual void saveToDatabase();

  protected:

    virtual QSharedPointer<Food> getCanonicalSharedPointer();

    virtual QSharedPointer<const Food> getCanonicalSharedPointer() const;

  private:

    Meal(int id, int creatorUserId, const QString& name, int userId,
         const QDate& date, const QVector<FoodAmount>& components,
         int temporaryId = -1);

    int id;
    int creatorUserId; // TODO: Replace with user object when created
    int userId;        // TODO: Replace with user object when created
    QDate date;
    int temporaryId;   // If >= 0, then temporary and not backed by database

    static QMap<int, QMap<QDate, QMap<int, QWeakPointer<Meal> > > > mealCache;

    static int nextTemporaryId;

    // Even though they are not backed by the database, a cache is needed for
    // temporary meals because they still require canonical pointers
    static QMap<int, QWeakPointer<Meal> > temporaryMealCache;

};

#endif /* MEAL_H_ */
