#include "nutrition_tracker_main.h"
#include <QtGui>
#include "nutrition_tracker_application.h"

#ifdef WIN32
  #include <winsock2.h>
  #include <iostream>
#endif

int main(int argc, char *argv[])
{
#ifdef WIN32
    WSADATA wsaData;

    if (WSAStartup(MAKEWORD(2,0), &wsaData) != 0) {
      std::cerr << "WSAStartup failed." << std::endl;
      return 1;
    }
#endif

    NutritionTrackerApplication a(argc, argv);
    NutritionTrackerMain w;

    QCoreApplication::setOrganizationName("Nerdland");
    QCoreApplication::setOrganizationDomain("nerdland.net");
    QCoreApplication::setApplicationName("Nutrition Tracker");

    w.show();

#ifdef WIN32
    int r = a.exec();
    WSACleanup();
    return r;
#else
    return a.exec();
#endif
}
