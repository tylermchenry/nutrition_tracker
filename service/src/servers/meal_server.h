#ifndef MEAL_SERVER_H_
#define MEAL_SERVER_H_

#include "libnutrition/proto/service/meal_messages.pb.h"
#include "libnutrition/data/meal.h"
#include <QString>
#include <QSet>

class MealLoadResponseObjects
{
  public:

    MealLoadResponseObjects();

    void addMeal(const QSharedPointer<const Meal>& meal);
    void addMeals(const QVector<QSharedPointer<Meal> >& meals);
    void addMeals(const QList<QSharedPointer<const Meal> >& meals);

    void setError(const QString& errorMessage = "");

    inline bool isEmpty() const { return meals.isEmpty(); }

    QList<QSharedPointer<const Meal> > getMeals() const;

    MealLoadResponse serialize() const;

  private:

    bool isError;
    QString errorMessage;
    QSet<Meal::MealIdTuple> mealIds;
    QList<QSharedPointer<const Meal> > meals;
};

class MealListing
{
  public:

    MealListing();

    void addMeal(const QSharedPointer<const Meal>& meal);
    void addMeal(int id, const QString& name);
    void addMeals(const QMap<int, QString>& mealNames);

    void setError(const QString& errorMessage = "");

    MealLoadResponse serialize() const;

  private:

    bool isError;
    QString errorMessage;
    QMap<int, QString> mealNames;
};

namespace MealServer
{
  MealLoadResponseObjects loadMeals
    (const MealLoadRequest& req, int loggedInUserId);

  MealListing loadMealNames
    (const MealLoadRequest& req, int loggedInUserId);
}

#endif /* MEAL_SERVER_H_ */
