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
	connect(ui.ftFoodsForDay, SIGNAL(contentsModified()), this, SLOT(updateBalance()));
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

  // Our food energy is stored in kilocalories, but the more accurate energy
  // density information is given in kilojoules (which are a smaller unit, so
  // more granularity). Therefore, compute by converting from kJ to kCal.
  // For future reference, alcohol is 29 kJ/g.

  static const double KCAL_PER_KJ = 0.239005736;
  static const double FAT_KCAL_PER_GRAM = 37 * KCAL_PER_KJ;
  static const double CARB_KCAL_PER_GRAM = 17 * KCAL_PER_KJ;
  static const double PROTEIN_KCAL_PER_GRAM = 17 * KCAL_PER_KJ;

  double pctCaloriesFromFat =  gramsFat * FAT_KCAL_PER_GRAM / totalCalories;
  double pctCaloriesFromCarbs =  gramsCarbs * CARB_KCAL_PER_GRAM / totalCalories;
  double pctCaloriesFromProtein =  gramsProtein * PROTEIN_KCAL_PER_GRAM / totalCalories;

  ui.txtFromFat->setText(QString::number(100 * pctCaloriesFromFat, 'f', 1) + " %");
  ui.txtFromCarbs->setText(QString::number(100 * pctCaloriesFromCarbs, 'f', 1) + " %");
  ui.txtFromProtein->setText(QString::number(100 * pctCaloriesFromProtein, 'f', 1) + " %");
}

void NutritionTracker::loadCurrentDayFoodsFromDatabase()
{
  QVector<QSharedPointer<Meal> > meals = Meal::getMealsForDay(1, getSelectedDate());

  qDebug() << "Got " << meals.size() << " meals for day " << getSelectedDate();

  ui.ftFoodsForDay->addMeals(meals);
}

