#ifndef ADD_FOOD_H
#define ADD_FOOD_H

#include <QtGui/QDialog>
#include <QDate>
#include "ui_add_food.h"
#include "libnutrition/data/food_collection.h"
#include "libnutrition/data/meal.h"

class AddFood : public QDialog
{
    Q_OBJECT

  public:
    explicit AddFood(const QDate& date, QWidget *parent = 0);
    ~AddFood();

  signals:

    void foodCollectionAdded(const QSharedPointer<const FoodCollection>& foodCollection);
    void mealsAdded(const QVector<QSharedPointer<const Meal> >& meals);

  private slots:

    void commitFoods();

  private:

    Ui::AddFoodClass ui;

    QDate date;
};

#endif // ADD_FOOD_H
