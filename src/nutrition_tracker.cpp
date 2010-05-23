#include "nutrition_tracker.h"
#include "model/food_tree_model.h"
#include <QDebug>

NutritionTracker::NutritionTracker(QWidget *parent)
    : QWidget(parent)
{
	ui.setupUi(this);

	connect(ui.calCurrentDay, SIGNAL(selectionChanged()), this, SLOT(changeDay()));

	ui.trvFoodsForDay->header()->setResizeMode(QHeaderView::ResizeToContents);
}

NutritionTracker::~NutritionTracker()
{
}

void NutritionTracker::initialize()
{
  changeDay();
}

QDate NutritionTracker::getSelectedDate() const
{
  return ui.calCurrentDay->selectedDate();
}

void NutritionTracker::addMealsToCurrentDay
  (const QVector<QSharedPointer<const Meal> >& meals)
{
  addMealsToCurrentDay(meals, true);
}

void NutritionTracker::changeDay()
{
  FoodTreeModel* foodTreeModel =
     new FoodTreeModel(ui.trvFoodsForDay, getSelectedDate(), getSelectedDate().toString());

  connect(foodTreeModel, SIGNAL(newGroupingCreated(const QModelIndex&)),
          this, SLOT(expandGrouping(const QModelIndex&)));

  ui.trvFoodsForDay->setModel(foodTreeModel);

  loadCurrentDayFoodsFromDatabase();
}

void NutritionTracker::expandGrouping(const QModelIndex& index)
{
  ui.trvFoodsForDay->setExpanded(index, true);
}

void NutritionTracker::addMealsToCurrentDay
  (const QVector<QSharedPointer<const Meal> >& meals, bool save)
{
  for (QVector<QSharedPointer<const Meal> >::const_iterator i = meals.begin(); i != meals.end(); ++i) {
    static_cast<FoodTreeModel*>(ui.trvFoodsForDay->model())->addMeal(*i);
  }

  if (save) saveCurrentDayFoodsToDatabase();
}

void NutritionTracker::loadCurrentDayFoodsFromDatabase()
{
  QVector<QSharedPointer<Meal> > meals = Meal::getMealsForDay(1, getSelectedDate());

  qDebug() << "Got " << meals.size() << " meals for day " << getSelectedDate();

  for (QVector<QSharedPointer<Meal> >::const_iterator i = meals.begin(); i != meals.end(); ++i) {
    static_cast<FoodTreeModel*>(ui.trvFoodsForDay->model())->addMeal(*i);
  }
}

void NutritionTracker::saveCurrentDayFoodsToDatabase()
{
  QVector<QSharedPointer<const Meal> > meals =
    static_cast<FoodTreeModel*>(ui.trvFoodsForDay->model())->getAllMeals();

  qDebug() << "Selected date is " << getSelectedDate();

  for (QVector<QSharedPointer<const Meal> >::const_iterator i = meals.begin(); i != meals.end(); ++i) {

    QSharedPointer<Meal> savedMeal =
      Meal::getOrCreateMeal((*i)->getUserId(), getSelectedDate(), (*i)->getMealId());

    savedMeal->clearComponents();
    savedMeal->mergeMeal(*i);
    savedMeal->saveToDatabase();
  }
}
