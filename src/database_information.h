#ifndef DATABASE_INFORMATION_H
#define DATABASE_INFORMATION_H

#include <QtGui/QDialog>
#include <QtSql/QSqlDatabase>
#include "ui_database_information.h"
#include <QByteArray>

class DatabaseInformation : public QDialog
{
    Q_OBJECT

  public:

    DatabaseInformation(QWidget *parent = 0);

    ~DatabaseInformation();

    QSqlDatabase getDatabase() const;

    bool tryAutoConnect();

  public slots:

    void makeConnection();

  private:

    Ui::DatabaseInformationClass ui;

    static QByteArray frobPassword(const QString& pw);
    static QString unfrobPassword(QByteArray fpwByteArray);
    static QByteArray& frob(QByteArray& byteArray);
};

#endif // DATABASE_INFORMATION_H
