#ifndef NUTRITION_TRACKER_H
#define NUTRITION_TRACKER_H

#include <QtGui/QWidget>
#include "ui_nutrition_tracker.h"
#include "libnutrition/data/meal.h"

class NutritionTracker : public QWidget
{
    Q_OBJECT

  public:

    NutritionTracker(QWidget *parent = 0);
    ~NutritionTracker();

    void initialize();

    QDate getSelectedDate() const;

  public slots:

    void refresh();

    void addMealsToCurrentDay(const QVector<QSharedPointer<const Meal> >& meals);

    void addFoodToCurrentDay(const FoodAmount& foodAmount, int mealId);

  signals:

    void addFood();
    void addFoodFromTemplate();
    void createFood();
    void createCompositeFood();


  private slots:

    void changeDay();

    void updateBalance();

  private:

    Ui::NutritionTrackerUI ui;

    void addMealsToCurrentDay(const QVector<QSharedPointer<const Meal> >& meals, bool save);

    void loadCurrentDayFoodsFromDatabase();

};

#endif // NUTRITION_TRACKER_H
