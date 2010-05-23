#ifndef ADD_FOOD_H
#define ADD_FOOD_H

#include <QtGui/QDialog>
#include <QtSql/QSqlDatabase>
#include <QDate>
#include "ui_add_food.h"
#include "data/food_collection.h"
#include "data/meal.h"

class AddFood : public QDialog
{
    Q_OBJECT

  public:
    AddFood(const QSqlDatabase& db, const QDate& date, QWidget *parent = 0);
    ~AddFood();

  signals:

    void foodCollectionAdded(const QSharedPointer<const FoodCollection>& foodCollection);
    void mealsAdded(const QVector<QSharedPointer<const Meal> >& meals);

  private slots:

    void expandGrouping(const QModelIndex& index);
    void clearFoods();
    void commitFoods();

  private:

    Ui::AddFoodClass ui;

    QDate date;
    QSqlDatabase db;
};

#endif // ADD_FOOD_H
