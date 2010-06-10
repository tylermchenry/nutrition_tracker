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
  ui.ftFoodsForDay->addMeals(meals);
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

  QMap<QString, NutrientAmount> nutrients = foodsForDay->getNutrients();

  double totalCalories = nutrients[Nutrient::getNutrientByName("Calories")->getId()].getAmount();

  if (totalCalories == 0) {
    ui.txtFromFat->setText("");
    ui.txtFromCarbs->setText("");
    ui.txtFromProtein->setText("");
    return;
  }

  double gramsFat = nutrients[Nutrient::getNutrientByName("Total Fat")->getId()].getAmount
    (Unit::getPreferredUnit(Unit::Dimensions::Weight));

  double gramsCarbs = nutrients[Nutrient::getNutrientByName("Total Carbohydrate")->getId()].getAmount
      (Unit::getPreferredUnit(Unit::Dimensions::Weight));

  double gramsProtein = nutrients[Nutrient::getNutrientByName("Protein")->getId()].getAmount
      (Unit::getPreferredUnit(Unit::Dimensions::Weight));

  const double FAT_CALORIES_PER_GRAM = 9;
  const double CARB_CALORIES_PER_GRAM = 4;
  const double PROTEIN_CALORIES_PER_GRAM = 4;

  ui.txtFromFat->setText(QString::number(100 * gramsFat * FAT_CALORIES_PER_GRAM / totalCalories, 'f', 1) + " %");
  ui.txtFromCarbs->setText(QString::number(100 * gramsCarbs * CARB_CALORIES_PER_GRAM / totalCalories, 'f', 1) + " %");
  ui.txtFromProtein->setText(QString::number(100 * gramsProtein * PROTEIN_CALORIES_PER_GRAM / totalCalories, 'f', 1) + " %");
}

void NutritionTracker::loadCurrentDayFoodsFromDatabase()
{
  QVector<QSharedPointer<Meal> > meals = Meal::getMealsForDay(1, getSelectedDate());

  qDebug() << "Got " << meals.size() << " meals for day " << getSelectedDate();

  ui.ftFoodsForDay->addMeals(meals);
}

