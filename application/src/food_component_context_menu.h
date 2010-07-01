/*
 * food_component_context_menu.h
 *
 *  Created on: May 26, 2010
 *      Author: tyler
 */

#ifndef FOOD_COMPONENT_CONTEXT_MENU_H_
#define FOOD_COMPONENT_CONTEXT_MENU_H_

#include "food_context_menu.h"
#include <QModelIndex>
#include "libnutrition/data/food_component.h"

class FoodComponentContextMenu : public FoodContextMenu
{
    Q_OBJECT;

  public:

    FoodComponentContextMenu(const QModelIndex& index, FoodComponent* component, QWidget* parent = 0);

    virtual ~FoodComponentContextMenu();

  private slots:

    virtual void actionTriggered(QAction* action);

  private:

    FoodComponent* component;

    QAction* actViewNutritionInfo;
    QAction* actChangeAmount;
    QMap<QAction*, QSharedPointer<const Unit> > actChangeUnit;
    QAction* actEdit;
    QAction* actDuplicate;
    QMap<QAction*, int> actMoveToMeal;
    QAction* actRemove;

    QMenu* mnuChangeUnit;
    QMenu* mnuMoveToMeal;

    template<typename T>
      void createAction(QMap<QAction*, T>& actionMap, const QString& label,
                          const T& mapVal, QMenu* menu = NULL, bool enabled = true);

    void createAction(QAction*& action, const QString& label, QMenu* menu = NULL,
                        bool enabled = true);
};

template<typename T>
   void FoodComponentContextMenu::createAction(QMap<QAction*, T>& actionMap, const QString& label,
                                           const T& mapVal, QMenu* menu, bool enabled)
{
  QAction* action;
  createAction(action, label, menu, enabled);
  actionMap[action] = mapVal;
}

#endif /* FOOD_COMPONENT_CONTEXT_MENU_H_ */
