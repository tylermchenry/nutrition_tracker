/*
 * nutrition_tracker_application.h
 * Part of nutrition_tracker
 *
 *  Created on: Jun 27, 2010
 *      Author: Tyler McHenry <tyler@nerdland.net>
 */

#ifndef NUTRITION_TRACKER_APPLICATION_H_
#define NUTRITION_TRACKER_APPLICATION_H_

#include <QApplication>

class NutritionTrackerApplication : public QApplication
{
  Q_OBJECT;

  public:

    NutritionTrackerApplication(int &c, char **v);

    virtual ~NutritionTrackerApplication();

    virtual bool notify(QObject *rec, QEvent *ev);
};

#endif /* NUTRITION_TRACKER_APPLICATION_H_ */
