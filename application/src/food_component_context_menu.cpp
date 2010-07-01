/*
 * food_component_context_menu.cpp
 *
 *  Created on: May 26, 2010
 *      Author: tyler
 */

#include "food_component_context_menu.h"
#include "libnutrition/data/food.h"
#include "libnutrition/data/food_collection.h"
#include "libnutrition/data/meal.h"
#include <stdexcept>
#include <QDebug>
#include <QtGui/QMessageBox>

FoodComponentContextMenu::FoodComponentContextMenu
  (const QModelIndex& index, FoodComponent* component, QWidget* parent)
  : FoodContextMenu(index, parent), component(component)
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

  // TODO: Enable action when implemented
  createAction(actEdit, "Edit...", this, false);

  addSeparator();

  // TODO: Enable action when implemented
  createAction(actDuplicate, "Duplicate", this, false);

  // TODO: Enable action when implemented
  mnuMoveToMeal = new QMenu("Move to Meal", this);
  mnuMoveToMeal->setEnabled(false);

  QMap<int, QString> mealNames = Meal::getAllMealNames();

  for(QMap<int, QString>::const_iterator i = mealNames.begin(); i != mealNames.end(); ++i) {
    // TODO: Enable action when implemented
    createAction(actMoveToMeal, i.value(), i.key(), mnuMoveToMeal, false);
  }

  addMenu(mnuMoveToMeal);

  createAction(actRemove, "Remove");

  connect(this, SIGNAL(triggered(QAction*)), this, SLOT(actionTriggered(QAction*)));
}

FoodComponentContextMenu::~FoodComponentContextMenu()
{
}

void FoodComponentContextMenu::actionTriggered(QAction* action)
{
  if (action == actViewNutritionInfo) {
    emit viewNutritionInformation(getIndex(), component->getFoodAmount());
  } else if (action == actChangeAmount) {
    emit changeAmount(getIndex(), component);
  } else if (actChangeUnit.contains(action)) {
    emit changeUnit(getIndex(), component, actChangeUnit[action]);
  } else if (action == actEdit) {
    emit edit(getIndex(), component);
  } else if (action == actDuplicate) {
    emit duplicate(getIndex(), component);
  } else if (actMoveToMeal.contains(action)) {
    emit moveToMeal(getIndex(), component, actMoveToMeal[action]);
  } else if (action == actRemove) {
    emit remove(getIndex(), component);
  }
}

void FoodComponentContextMenu::createAction(QAction*& action, const QString& label,
                                                  QMenu* menu, bool enabled)
{
    action = new QAction(label, (menu ? menu : this));
    action->setEnabled(enabled);
    (menu ? menu : this)->addAction(action);
}
