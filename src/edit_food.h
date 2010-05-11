#ifndef EDIT_FOOD_H
#define EDIT_FOOD_H

#include <QtGui/QDialog>
#include "ui_edit_food.h"

class EditFood : public QDialog
{
    Q_OBJECT

public:
    EditFood(QWidget *parent = 0);
    ~EditFood();

private:
    Ui::EditFoodUI ui;
};

#endif // EDIT_FOOD_H
