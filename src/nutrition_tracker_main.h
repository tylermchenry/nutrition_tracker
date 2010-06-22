#ifndef NUTRITION_TRACKER_MAIN_H
#define NUTRITION_TRACKER_MAIN_H

#include <QtGui/QMainWindow>
#include <QtSql/QSqlDatabase>
#include "ui_nutrition_tracker_main.h"
#include "nutrition_tracker.h"

class NutritionTrackerMain : public QMainWindow
{
    Q_OBJECT

  public:
    NutritionTrackerMain(QWidget *parent = 0);
    ~NutritionTrackerMain();

  public slots:
    void show();
    void showAddFood();
    void showCreateFood();
    void showCreateCompositeFood();
    void showCreateTemplate();
    void showMyFoods();

  private:
    Ui::NutritionTrackerMainUI ui;
    NutritionTracker* tracker;
    QSqlDatabase db;
};

#endif // NUTRITION_TRACKER_H
