#ifndef ADD_FOOD_H
#define ADD_FOOD_H

#include <QtGui/QDialog>
#include <QtSql/QSqlDatabase>
#include "ui_add_food.h"

class AddFood : public QDialog
{
    Q_OBJECT

public:
    AddFood(const QSqlDatabase& db, QWidget *parent = 0);
    ~AddFood();

public slots:

  void clearSearchResults();
  void addSearchResult(int id, const QString& type, const QString& description);
  void updateAddControls(QListWidgetItem* curSelectedItem);

private:
    Ui::AddFoodClass ui;

    QSqlDatabase db;
};

#endif // ADD_FOOD_H
