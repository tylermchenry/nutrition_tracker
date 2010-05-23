#ifndef NUTRITION_TRACKER_H
#define NUTRITION_TRACKER_H

#include <QtGui/QWidget>
#include "ui_nutrition_tracker.h"
#include "data/meal.h"

class NutritionTracker : public QWidget
{
    Q_OBJECT

  public:

    NutritionTracker(QWidget *parent = 0);
    ~NutritionTracker();

    void initialize();

    QDate getSelectedDate() const;

  public slots:

    void addMealsToCurrentDay(const QVector<QSharedPointer<const Meal> >& meals);

  private slots:

    void changeDay();
    void expandGrouping(const QModelIndex& index);

  private:

    Ui::NutritionTrackerUI ui;

    void addMealsToCurrentDay(const QVector<QSharedPointer<const Meal> >& meals, bool save);

    void loadCurrentDayFoodsFromDatabase();
    void saveCurrentDayFoodsToDatabase();

};

#endif // NUTRITION_TRACKER_H
