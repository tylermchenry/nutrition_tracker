/*
 * food_context_menu.h
 *
 *  Created on: May 26, 2010
 *      Author: tyler
 */

#ifndef FOOD_CONTEXT_MENU_H_
#define FOOD_CONTEXT_MENU_H_

#include <QtGui/QMenu>
#include <QModelIndex>
#include "data/food_component.h"

class FoodContextMenu : public QMenu
{
    Q_OBJECT;

  public:

    FoodContextMenu(const QModelIndex& index, FoodComponent* component, QWidget* parent = 0);

    virtual ~FoodContextMenu();

  signals:

    void viewNutritionInformation(const QModelIndex& index, FoodComponent* component);
    void changeAmount(const QModelIndex& index, FoodComponent* component);
    void changeUnit(const QModelIndex& index, FoodComponent* component, const QSharedPointer<const Unit>& unit);
    void edit(const QModelIndex& index, FoodComponent* component);
    void duplicate(const QModelIndex& index, FoodComponent* component);
    void moveToMeal(const QModelIndex& index, FoodComponent* component, int mealId);
    void remove(const QModelIndex& index, FoodComponent* component);

  private slots:

    void actionTriggered(QAction* action);

  private:

    QModelIndex index;
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
                          const T& mapVal, QMenu* menu = NULL);

    void createAction(QAction*& action, const QString& label, QMenu* menu = NULL);
};

template<typename T>
   void FoodContextMenu::createAction(QMap<QAction*, T>& actionMap, const QString& label,
                                           const T& mapVal, QMenu* menu)
{
  QAction* action;
  createAction(action, label, menu);
  actionMap[action] = mapVal;
}

#endif /* FOOD_CONTEXT_MENU_H_ */
