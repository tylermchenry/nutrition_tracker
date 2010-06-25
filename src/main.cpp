#include "nutrition_tracker_main.h"

#include <QtGui>
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    NutritionTrackerMain w;

    QCoreApplication::setOrganizationName("Nerdland");
    QCoreApplication::setOrganizationDomain("nerdland.net");
    QCoreApplication::setApplicationName("Nutrition Tracker");

    w.show();
    return a.exec();
}
