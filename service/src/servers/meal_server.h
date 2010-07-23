#ifndef MEAL_SERVER_H_
#define MEAL_SERVER_H_

#include "libnutrition/proto/service/meal_messages.pb.h"
#include "libnutrition/data/meal.h"
#include "servers/response_objects.h"
#include <QString>
#include <QSet>

class MealLoadResponseObjects : public ResponseObjects<Meal, MealLoadResponse>
{
  protected:

    virtual Meal::MealIdTuple getId
      (const QSharedPointer<const Meal>& meal) const
        { return meal->getMealIdTuple(); }

    virtual void addObjectToResponse
      (MealLoadResponse& resp, const QSharedPointer<const Meal>& meal) const
        { *(resp.add_meals()) = meal->serialize(); }
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
