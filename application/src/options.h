#ifndef OPTIONS_H
#define OPTIONS_H

#include <QtGui/QDialog>
#include "ui_options.h"
#include <QByteArray>
#include <QString>

class Options : public QDialog
{
    Q_OBJECT

  public:

    Options(QWidget *parent = 0);

    ~Options();

  private slots:

    void saveOptionsAndClose();
    bool saveOptions();
    void toggleDBOptions(int state);

  private:

    Ui::OptionsUI ui;

    void loadOptions();

    static QByteArray frobPassword(const QString& pw);
    static QString unfrobPassword(QByteArray fpwByteArray);
    static QByteArray& frob(QByteArray& byteArray);
};

#endif // OPTIONS_H
