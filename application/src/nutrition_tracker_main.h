#ifndef NUTRITION_TRACKER_MAIN_H
#define NUTRITION_TRACKER_MAIN_H

#include <QtGui/QMainWindow>
#include <QtSql/QSqlDatabase>
#include "ui_nutrition_tracker_main.h"
#include "widgets/nutrition_tracker.h"
#include "libnutrition/data/food_amount.h"

class NutritionTrackerMain : public QMainWindow
{
    Q_OBJECT

  public:

    NutritionTrackerMain(QWidget *parent = 0);
    ~NutritionTrackerMain();

  public slots:

    void show();
    void showAddFood();
    void showAddFoodFromTemplate();
    void showCreateFood();
    void showCreateCompositeFood();
    void showCreateCompositeFoodFromTemplate();
    void showCreateTemplate();
    void showMyFoods();
    void showOptions();
    void showAbout();

  private:

    inline FoodAmount promptForTemplateInstance()
      { int m; return promptForTemplateInstance(false, m); }

    FoodAmount promptForTemplateInstance(bool promptForMeal, int& mealId);

    Ui::NutritionTrackerMainUI ui;
    NutritionTracker* tracker;
    QSqlDatabase db;
};

#endif // NUTRITION_TRACKER_H
