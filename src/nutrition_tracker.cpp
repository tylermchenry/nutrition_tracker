#include "nutrition_tracker.h"
#include "food_context_menu.h"
#include "model/food_tree_model.h"
#include "data/food_collection.h"
#include <QDebug>
#include <QtGui/QCursor>

NutritionTracker::NutritionTracker(QWidget *parent)
    : QWidget(parent)
{
	ui.setupUi(this);

	connect(ui.calCurrentDay, SIGNAL(selectionChanged()), this, SLOT(changeDay()));
}

NutritionTracker::~NutritionTracker()
{
}

void NutritionTracker::initialize()
{
  ui.ftFoodsForDay->initialize();
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
  QDate date = getSelectedDate();

  ui.ftFoodsForDay->setDate(date);
  ui.ftFoodsForDay->setRootName(date.toString());
  ui.ftFoodsForDay->clear();

  loadCurrentDayFoodsFromDatabase();
}

void NutritionTracker::addMealsToCurrentDay
  (const QVector<QSharedPointer<const Meal> >& meals, bool save)
{
  ui.ftFoodsForDay->addMeals(meals);

  if (save) saveCurrentDayFoodsToDatabase();
}

void NutritionTracker::loadCurrentDayFoodsFromDatabase()
{
  QVector<QSharedPointer<Meal> > meals = Meal::getMealsForDay(1, getSelectedDate());

  qDebug() << "Got " << meals.size() << " meals for day " << getSelectedDate();

  ui.ftFoodsForDay->addMeals(meals);
}

void NutritionTracker::saveCurrentDayFoodsToDatabase()
{
  QVector<QSharedPointer<const Meal> > meals = ui.ftFoodsForDay->getAllMeals();

  qDebug() << "Selected date is " << getSelectedDate();

  for (QVector<QSharedPointer<const Meal> >::const_iterator i = meals.begin(); i != meals.end(); ++i) {

    QSharedPointer<Meal> savedMeal =
      Meal::getOrCreateMeal((*i)->getUserId(), getSelectedDate(), (*i)->getMealId());

    savedMeal->replaceWith(*i);
    savedMeal->saveToDatabase();
  }
}
