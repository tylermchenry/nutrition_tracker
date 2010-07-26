#include "nutrition_tracker.h"
#include "menus/food_context_menu.h"
#include "model/food_tree_model.h"
#include "libnutrition/data/food_collection.h"
#include "libnutrition/data/composite_food.h"
#include "libnutrition/data/nutrient.h"
#include "libnutrition/data/group.h"
#include "libnutrition/data/unit.h"
#include <QDebug>
#include <QtGui/QCursor>

NutritionTracker::NutritionTracker(QWidget *parent)
    : QWidget(parent)
{
  ui.setupUi(this);

  connect(ui.calCurrentDay, SIGNAL(selectionChanged()), this, SLOT(changeDay()));
  connect(ui.ftFoodsForDay, SIGNAL(contentsModified()), this, SLOT(updateBalance()));

  connect(ui.btnAddFood, SIGNAL(clicked()), this, SIGNAL(addFood()));
  connect(ui.btnAddFoodFromTemplate, SIGNAL(clicked()), this, SIGNAL(addFoodFromTemplate()));
  connect(ui.btnCreateFood, SIGNAL(clicked()), this, SIGNAL(createFood()));
  connect(ui.btnCreateCompositeFood, SIGNAL(clicked()), this, SIGNAL(createCompositeFood()));
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

void NutritionTracker::refresh()
{
  changeDay();
}

void NutritionTracker::addMealsToCurrentDay
  (const QVector<QSharedPointer<const Meal> >& meals)
{
  ui.ftFoodsForDay->addMeals(meals);
  updateBalance();
}

void NutritionTracker::addFoodToCurrentDay(const FoodAmount& foodAmount, int mealId)
{
  ui.ftFoodsForDay->addFoodAmount(foodAmount, mealId);
  updateBalance();
}

void NutritionTracker::changeDay()
{
  QDate date = getSelectedDate();

  ui.ftFoodsForDay->setTemporary(false);
  ui.ftFoodsForDay->setDate(date);
  ui.ftFoodsForDay->setRootName(date.toString());
  ui.ftFoodsForDay->clear();

  loadCurrentDayFoodsFromDatabase();
  updateBalance();
}

void NutritionTracker::updateBalance()
{
  // TODO: Clean this up a bit, and maybe include alcohol?

  QSharedPointer<const FoodCollection> foodsForDay = ui.ftFoodsForDay->getAllFoods();

  NutrientAmount totalCalories =
    foodsForDay->getCaloriesFromNutrientName(Nutrient::CALORIES_NAME);

  if (totalCalories.getAmount() == 0) {
    ui.txtFromFat->setText("");
    ui.txtFromCarbs->setText("");
    ui.txtFromProtein->setText("");
    ui.txtFromOther->setText("");
    return;
  }

  NutrientAmount caloriesFromFat =
    foodsForDay->getCaloriesFromNutrientName(Nutrient::FAT_NAME);

  NutrientAmount caloriesFromCarbs =
    foodsForDay->getCaloriesFromNutrientName(Nutrient::CARBOHYDRATE_NAME);

  NutrientAmount caloriesFromProtein =
    foodsForDay->getCaloriesFromNutrientName(Nutrient::PROTEIN_NAME);

  double pctCaloriesFromFat = caloriesFromFat.getAmount() / totalCalories.getAmount();
  double pctCaloriesFromCarbs = caloriesFromCarbs.getAmount() / totalCalories.getAmount();
  double pctCaloriesFromProtein = caloriesFromProtein.getAmount() / totalCalories.getAmount();
  double pctCaloriesFromOther = 1 - pctCaloriesFromFat - pctCaloriesFromCarbs -
      pctCaloriesFromProtein;

  if (pctCaloriesFromOther < 0) pctCaloriesFromOther = 0;

  ui.txtFromFat->setText(QString::number(100 * pctCaloriesFromFat, 'f', 1) + " %");
  ui.txtFromCarbs->setText(QString::number(100 * pctCaloriesFromCarbs, 'f', 1) + " %");
  ui.txtFromProtein->setText(QString::number(100 * pctCaloriesFromProtein, 'f', 1) + " %");
  ui.txtFromOther->setText(QString::number(100 * pctCaloriesFromOther, 'f', 1) + " %");
}

void NutritionTracker::loadCurrentDayFoodsFromDatabase()
{
  QVector<QSharedPointer<Meal> > meals = Meal::getMealsForDay
    (User::getLoggedInUser()->getId(), getSelectedDate());

  qDebug() << "Got " << meals.size() << " meals for day " << getSelectedDate();

  ui.ftFoodsForDay->addMeals(meals);
}

