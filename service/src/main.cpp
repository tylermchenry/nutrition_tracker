#include <QtCore>
#include "nutrition_server_application.h"

int main(int argc, char *argv[])
{
    NutritionServerApplication a(argc, argv);

    QCoreApplication::setOrganizationName("Nerdland");
    QCoreApplication::setOrganizationDomain("nerdland.net");
    QCoreApplication::setApplicationName("Nutrition Tracking Service");

    return a.exec();
}
