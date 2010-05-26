/*
 * food_context_menu.h
 *
 *  Created on: May 26, 2010
 *      Author: tyler
 */

#ifndef FOOD_CONTEXT_MENU_H_
#define FOOD_CONTEXT_MENU_H_

#include <QtGui/QMenu>
#include <QSignalMapper>
#include "data/food_component.h"

class FoodContextMenu : public QMenu
{
    Q_OBJECT;

  public:

    explicit FoodContextMenu(FoodComponent* component, QWidget* parent = 0);

    virtual ~FoodContextMenu();

  signals:

    void viewNutritionInformation(FoodComponent* component);
    void changeAmount(FoodComponent* component);
    void changeUnit(FoodComponent* component, const QSharedPointer<const Unit>& unit);
    void edit(FoodComponent* component);
    void duplicate(FoodComponent* component);
    void moveToMeal(FoodComponent* component, int mealId);
    void remove(FoodComponent* component);

  private slots:

    void actionTriggered(QWidget* widget);

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

    QSignalMapper* signalMapper;

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
