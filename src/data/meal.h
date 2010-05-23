/*
 * meal.h
 * Part of nutrition_tracker
 *
 *  Created on: May 22, 2010
 *      Author: Tyler McHenry <tyler@nerdland.net>
 */

#ifndef MEAL_H_
#define MEAL_H_

#include "composite_food.h"
#include <QDate>
#include <QMap>

class Meal : public CompositeFood
{
  public:

    static QMap<int, QString> getAllMealNames
      (int creatorUserId = -1, bool includeGenerics = true);

    static QSharedPointer<Meal> getMeal(int userId, const QDate& date, int mealId);

    static QVector<QSharedPointer<Meal> > getMealsForDay(int userId, const QDate& date);

    static QSharedPointer<Meal> createMealFromQueryResults(QSqlQuery& query);

    virtual ~Meal();

    inline int getMealId() { return id; }

    inline QDate getDate() { return date; }

    virtual void saveToDatabase();

  protected:

    virtual QSharedPointer<Food> getCanonicalSharedPointer();

    virtual QSharedPointer<const Food> getCanonicalSharedPointer() const;

  private:

    Meal(int id, int creatorUserId, const QString& name, int userId,
         const QDate& date, const QVector<FoodAmount>& components);

    int id;
    int creatorUserId; // TODO: Replace with user object when created
    int userId;        // TODO: Replace with user object when created
    QDate date;

    static QMap<int, QMap<QDate, QMap<int, QWeakPointer<Meal> > > > mealCache;

};

#endif /* MEAL_H_ */
