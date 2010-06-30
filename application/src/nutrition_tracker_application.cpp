/*
 * nutrition_tracker_application.cpp
 * Part of nutrition_tracker
 *
 *  Created on: Jun 27, 2010
 *      Author: Tyler McHenry <tyler@nerdland.net>
 */

#include "nutrition_tracker_application.h"
#include <QtGui/QMessageBox>
#include <QDebug>
#include <stdexcept>

NutritionTrackerApplication::NutritionTrackerApplication(int &c, char **v)
  : QApplication(c, v)
{
}

NutritionTrackerApplication::~NutritionTrackerApplication()
{
}

bool NutritionTrackerApplication::notify(QObject *rec, QEvent *ev)
{
  try {
    return QApplication::notify(rec, ev);
  }
  catch (std::exception& ex) {
    qDebug() << "Fatal exception caught: " << ex.what();
    QMessageBox::critical(NULL, "Fatal Error", QString("Fatal Error: ") + ex.what());
    abort();
  }
  catch (...) {
    qDebug() << "Fatal exception not derived from std::exception caught.";
    QMessageBox::critical(NULL, "Fatal Error", "Fatal Error: No details available.");
    abort();
  }
}
