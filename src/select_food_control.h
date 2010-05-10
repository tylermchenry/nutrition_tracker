#ifndef SELECT_FOOD_CONTROL_H
#define SELECT_FOOD_CONTROL_H

#include <QtGui/QWidget>
#include "ui_select_food_control.h"

class SelectFoodControl : public QWidget
{
    Q_OBJECT

public:
    SelectFoodControl(QWidget *parent = 0);
    ~SelectFoodControl();

public slots:

    void clearFoodList();
    void addToFoodList(int id, const QString& type, const QString& description);
    void updateAddControls(QListWidgetItem* curSelectedItem);

private:
    Ui::SelectFoodControlUI ui;
};

#endif // SELECT_FOOD_CONTROL_H
