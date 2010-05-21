#ifndef FOOD_SEARCH_CONTROL_H
#define FOOD_SEARCH_CONTROL_H

#include <QtGui/QWidget>
#include <QtSql/QSqlDatabase>
#include <QMap>
#include <set>
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

    void runSearchQuery(const QString& queryText, std::set<Result>& results) const;
};

#endif // FOOD_SEARCH_CONTROL_H
