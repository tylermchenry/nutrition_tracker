#include <QtCore>
#include <QCoreApplication>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
	
    QCoreApplication::setOrganizationName("Nerdland");
    QCoreApplication::setOrganizationDomain("nerdland.net");
    QCoreApplication::setApplicationName("Nutrition Tracking Service");

    return a.exec();
}
