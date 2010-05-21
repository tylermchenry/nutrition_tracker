#ifndef FOOD_SEARCH_CONTROL_H
#define FOOD_SEARCH_CONTROL_H

#include <QtGui/QWidget>
#include <QtSql/QSqlDatabase>
#include <QMap>
#include <QSet>
#include "ui_food_search_control.h"

class FoodSearchControl : public QWidget
{
    Q_OBJECT

public:

    struct Result
    {
      int id;
      QString type;
      QString description; /* Sorted ONLY by description */

      explicit Result(int i = 0, const QString& t = "", const QString& d = "")
        : id(i), type(t), description(d)
      {}

      bool operator< (const Result& rhs) const
      { return description < rhs.description; }

      bool operator== (const Result& rhs) const
      { return description == rhs.description; }
    };

    FoodSearchControl(QWidget *parent = 0);
    ~FoodSearchControl();

    void setDatabase(const QSqlDatabase& db);

public slots:

  void performSearch();

signals:

  void beginNewSearch();

  void newResult(const FoodSearchControl::Result& result);

private:

    Ui::FoodSearchControlUI ui;
    QSqlDatabase db;
    QMap<QString, QString> categoryToGroupID;

    void runSearchQuery(const QString& queryText, QSet<Result>& results) const;
};

uint qHash(const FoodSearchControl::Result& result);

#endif // FOOD_SEARCH_CONTROL_H
