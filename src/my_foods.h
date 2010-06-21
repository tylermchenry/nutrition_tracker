#ifndef MY_FOODS_H
#define MY_FOODS_H

#include <QtGui/QWidget>
#include "ui_my_foods.h"
#include "data/food_collection.h"

class MyFoods : public QDialog
{
    Q_OBJECT

  public:
    MyFoods(QWidget *parent = 0);
    ~MyFoods();

  private slots:

    void edit();
    void duplicate();
    void del();

    void loadUserFoods();

  private:

    void reportDBError();

    Ui::MyFoodsUI ui;

    QMap<int, QPair<FoodCollection::ContainedTypes::ContainedType, int> >
      itemToFood;
};

#endif // MY_FOODS_H
