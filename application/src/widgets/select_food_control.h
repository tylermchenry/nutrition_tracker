#ifndef SELECT_FOOD_CONTROL_H
#define SELECT_FOOD_CONTROL_H

#include <QtGui/QWidget>
#include "ui_select_food_control.h"
#include "food_search_control.h" // TODO: Get rid of dependence on this widget
#include "libnutrition/data/food.h"

class SelectFoodControl : public QWidget
{
    Q_OBJECT

public:
    explicit SelectFoodControl(QWidget *parent = 0);
    explicit SelectFoodControl(bool selectMeals, QWidget *parent = 0);
    ~SelectFoodControl();

    void setAllowMealSelection(bool selectMeals);

signals:

    void amountAdded(const FoodAmount& foodAmount, int mealId);

public slots:

    void clearFoodList();
    void addToFoodList(const FoodSearchControl::Result& result);
    void updateAddControls();

private slots:

    void addClicked();

private:

    Ui::SelectFoodControlUI ui;

    bool selectMeals;
    bool includeRefuse;

    QMap<int, FoodSearchControl::Result> itemToResult;
    QSharedPointer<Food> selectedFood;

    int specializedSeparatorRow;

    void initialize();
    void populateMealSelector(QComboBox* cbMeals);

};

#endif // SELECT_FOOD_CONTROL_H
