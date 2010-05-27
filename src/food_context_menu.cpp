/*
 * food_context_menu.cpp
 *
 *  Created on: May 26, 2010
 *      Author: tyler
 */

#include "food_context_menu.h"
#include "data/food.h"
#include "data/meal.h"
#include <stdexcept>
#include <QDebug>
#include <QtGui/QMessageBox>

FoodContextMenu::FoodContextMenu(FoodComponent* component, QWidget* parent)
  : QMenu("Food", parent)
{
  if (!component || !component->getFoodAmount().isDefined()) {
    throw std::logic_error("Attempted to create a context menu for an undefined food component.");
  }

  createAction(actViewNutritionInfo, "View Nutrition Information...");

  addSeparator();

  createAction(actChangeAmount, "Change Amount...");

  mnuChangeUnit = new QMenu("Change Unit", this);

  QList<Unit::Dimensions::Dimension> validDimensions =
     component->getFoodAmount().getFood()->getValidDimensions();

  for (QList<Unit::Dimensions::Dimension>::const_iterator i = validDimensions.begin();
      i != validDimensions.end(); ++i)
  {
    QVector<QSharedPointer<const Unit> > dimensionUnits = Unit::getAllUnits(*i);

    for (QVector<QSharedPointer<const Unit> >::const_iterator j = dimensionUnits.begin();
        j != dimensionUnits.end(); ++j)
    {
      createAction(actChangeUnit, (*j)->getName(), *j, mnuChangeUnit);
    }
  }

  addMenu(mnuChangeUnit);

  createAction(actEdit, "Edit...");

  addSeparator();

  createAction(actDuplicate, "Duplicate");

  mnuMoveToMeal = new QMenu("Move to Meal", this);

  QMap<int, QString> mealNames = Meal::getAllMealNames();

  for(QMap<int, QString>::const_iterator i = mealNames.begin(); i != mealNames.end(); ++i) {
    createAction(actMoveToMeal, i.value(), i.key(), mnuMoveToMeal);
  }

  addMenu(mnuMoveToMeal);

  createAction(actRemove, "Remove");

  connect(this, SIGNAL(triggered(QAction*)), this, SLOT(actionTriggered(QAction*)));
}

FoodContextMenu::~FoodContextMenu()
{
}

void FoodContextMenu::actionTriggered(QAction* action)
{
  if (action == actViewNutritionInfo) {
    emit viewNutritionInformation(component);
  } else if (action == actChangeAmount) {
    emit changeAmount(component);
  } else if (actChangeUnit.contains(action)) {
    emit changeUnit(component, actChangeUnit[action]);
  } else if (action == actEdit) {
    emit edit(component);
  } else if (action == actDuplicate) {
    emit duplicate(component);
  } else if (actMoveToMeal.contains(action)) {
    emit moveToMeal(component, actMoveToMeal[action]);
  } else if (action == actRemove) {
    emit remove(component);
  }
}

void FoodContextMenu::createAction(QAction*& action, const QString& label, QMenu* menu)
{
    action = new QAction(label, (menu ? menu : this));
    (menu ? menu : this)->addAction(action);
}
