#ifndef NUTRITION_TRACKER_H
#define NUTRITION_TRACKER_H

#include <QtGui/QWidget>
#include "ui_nutrition_tracker.h"

class NutritionTracker : public QWidget
{
    Q_OBJECT

public:
    NutritionTracker(QWidget *parent = 0);
    ~NutritionTracker();

private:
    Ui::NutritionTrackerUI ui;
};

#endif // NUTRITION_TRACKER_H
