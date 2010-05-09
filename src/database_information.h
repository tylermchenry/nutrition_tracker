#ifndef DATABASE_INFORMATION_H
#define DATABASE_INFORMATION_H

#include <QtGui/QDialog>
#include <QtSql/QSqlDatabase>
#include "ui_database_information.h"

class DatabaseInformation : public QDialog
{
    Q_OBJECT

public:
    DatabaseInformation(QWidget *parent = 0);
    ~DatabaseInformation();

  QSqlDatabase getDatabase() const;

public slots:

  void makeConnection();

private:
    Ui::DatabaseInformationClass ui;
};

#endif // DATABASE_INFORMATION_H
