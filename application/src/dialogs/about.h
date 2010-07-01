#ifndef ABOUT_H
#define ABOUT_H

#include <QtGui/QDialog>
#include "ui_about.h"

class About : public QDialog
{
    Q_OBJECT

public:
    About(QWidget *parent = 0);
    ~About();

private:
    Ui::AboutUI ui;
};

#endif // ABOUT_H
