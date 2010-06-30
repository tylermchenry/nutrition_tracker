/*
 * meal_context_menu.cpp
 * Part of nutrition_tracker
 *
 *  Created on: Jun 6, 2010
 *      Author: Tyler McHenry <tyler@nerdland.net>
 */

#include "meal_context_menu.h"
#include <QtGui/QInputDialog>
#include <QtGui/QMessageBox>

MealContextMenu::MealContextMenu
  (const QModelIndex& index, const QSharedPointer<Meal>& meal, QWidget* parent)
 : FoodContextMenu(index, parent), meal(meal)
{
  if (!meal) {
    throw std::logic_error("Attempted to create a context menu for an undefined meal.");
  }

  actViewNutritionInfo = new QAction("View Nutrition Information...", this);
  addAction(actViewNutritionInfo);

  if (!meal->isTemporary()) {

    addSeparator();

    mnuCopyToDay = new QMenu("Copy to Day", this);

    bool mealIsToday = (meal->getDate() == QDate::currentDate());

    QDate nextDay = meal->getDate().addDays(1);
    QAction* actCopyToNextDay = new QAction
        ((mealIsToday ? "Tomorrow" : "Next Day: " + nextDay.toString()), mnuCopyToDay);
    actCopyToDay.insert(actCopyToNextDay, nextDay);
    mnuCopyToDay->addAction(actCopyToNextDay);

    QDate prevDay = meal->getDate().addDays(-1);
    QAction* actCopyToPrevDay = new QAction
        ((mealIsToday ? "Yesterday" : "Previous Day: " + prevDay.toString()), mnuCopyToDay);
    actCopyToDay.insert(actCopyToPrevDay, prevDay);
    mnuCopyToDay->addAction(actCopyToPrevDay);

    QAction* actCopyToOther = new QAction("Other...", mnuCopyToDay);
    actCopyToDay.insert(actCopyToOther, QDate());
    mnuCopyToDay->addAction(actCopyToOther);

    addMenu(mnuCopyToDay);
  }

  connect(this, SIGNAL(triggered(QAction*)), this, SLOT(actionTriggered(QAction*)));
}

MealContextMenu::~MealContextMenu()
{
}

void MealContextMenu::actionTriggered(QAction* action)
{
  if (action == actViewNutritionInfo) {
    emit viewNutritionInformation(getIndex(), meal->getBaseAmount());
  } else if (actCopyToDay.contains(action)) {

    QDate date = actCopyToDay[action];

    if (date == QDate()) {
      QString dateStr = QInputDialog::getText(this, "Enter Date", "(temporary) Copy meal to date (YYYY-MM-DD):");
      if ((date = QDate::fromString(dateStr, Qt::ISODate)) == QDate()) {
        QMessageBox::critical(this, "Error", "Invalid Date.");
        return;
      } else if (date == meal->getDate()) {
        QMessageBox::critical(this, "Error", "Cannot copy a meal to its own date.");
        return;
      }
    }

    emit copyMealToDay(getIndex(), meal, date);
  }
}
