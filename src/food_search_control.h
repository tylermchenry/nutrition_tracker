#ifndef FOOD_SEARCH_CONTROL_H
#define FOOD_SEARCH_CONTROL_H

#include <QtGui/QWidget>
#include <QtSql/QSqlDatabase>
#include <QMap>
#include "ui_food_search_control.h"

class FoodSearchControl : public QWidget
{
    Q_OBJECT

public:
    FoodSearchControl(QWidget *parent = 0);
    ~FoodSearchControl();

    void setDatabase(const QSqlDatabase& db);

public slots:

  void performSearch();

signals:

  void beginNewSearch();

  void newResult(int id, const QString& type, const QString& description);

private:
    Ui::FoodSearchControlUI ui;
    QSqlDatabase db;
    QMap<QString, QString> categoryToGroupID;
};

#endif // FOOD_SEARCH_CONTROL_H
