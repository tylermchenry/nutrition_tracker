#ifndef SELECT_FOOD_CONTROL_H
#define SELECT_FOOD_CONTROL_H

#include <QtGui/QWidget>
#include "ui_select_food_control.h"
#include "food_search_control.h" // TODO: Get rid of dependence on this widget

class SelectFoodControl : public QWidget
{
    Q_OBJECT

public:
    SelectFoodControl(QWidget *parent = 0);
    ~SelectFoodControl();

public slots:

    void clearFoodList();
    void addToFoodList(const FoodSearchControl::Result& result);
    void updateAddControls(QListWidgetItem* curSelectedItem);

private:

    Ui::SelectFoodControlUI ui;

    QMap<int, FoodSearchControl::Result> itemToResult;

};

#endif // SELECT_FOOD_CONTROL_H
