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

FoodContextMenu::FoodContextMenu
  (const QModelIndex& index, FoodComponent* component, QWidget* parent)
  : QMenu("Food", parent), index(index), component(component)
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
    emit viewNutritionInformation(index, component);
  } else if (action == actChangeAmount) {
    emit changeAmount(index, component);
  } else if (actChangeUnit.contains(action)) {
    emit changeUnit(index, component, actChangeUnit[action]);
  } else if (action == actEdit) {
    emit edit(index, component);
  } else if (action == actDuplicate) {
    emit duplicate(index, component);
  } else if (actMoveToMeal.contains(action)) {
    emit moveToMeal(index, component, actMoveToMeal[action]);
  } else if (action == actRemove) {
    emit remove(index, component);
  }
}

void FoodContextMenu::createAction(QAction*& action, const QString& label, QMenu* menu)
{
    action = new QAction(label, (menu ? menu : this));
    (menu ? menu : this)->addAction(action);
}
