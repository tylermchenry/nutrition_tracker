#include "add_food.h"
#include "model/food_tree_model.h"
#include <QDebug>
#include <QSettings>

AddFood::AddFood(const QDate& date, QWidget *parent)
    : QDialog(parent), date(date)
{
  ui.setupUi(this);

  connect(ui.fscSearch, SIGNAL(beginNewSearch()),
          ui.sfcResultsList, SLOT(clearFoodList()));
  connect(ui.fscSearch, SIGNAL(newResult(const FoodSearchControl::Result&)),
          ui.sfcResultsList, SLOT(addToFoodList(const FoodSearchControl::Result&)));

  connect(ui.btnCancel, SIGNAL(clicked()), this, SLOT(close()));
  connect(ui.btnCommit, SIGNAL(clicked()), this, SLOT(commitFoods()));

  connect(ui.btnReset, SIGNAL(clicked()), ui.ftFoodsToAdd, SLOT(clear()));
  connect(ui.sfcResultsList, SIGNAL(amountAdded(const FoodAmount&, int)),
          ui.ftFoodsToAdd, SLOT(addFoodAmount(const FoodAmount&, int)));

  ui.ftFoodsToAdd->setTemporary(true);
  ui.ftFoodsToAdd->setDate(date);
  ui.ftFoodsToAdd->setRootName("Proposed Additions");
  ui.ftFoodsToAdd->initialize();

  QSettings settings("Nerdland", "Nutrition Tracker");
  settings.beginGroup("addfood");
  resize(settings.value("size", size()).toSize());
  if (!settings.value("position").isNull()) {
    move(settings.value("position", pos()).toPoint());
  }
  settings.endGroup();
}

AddFood::~AddFood()
{
  QSettings settings("Nerdland", "Nutrition Tracker");
  settings.beginGroup("addfood");
  settings.setValue("size", size());
  settings.setValue("position", pos());
  settings.endGroup();
}

void AddFood::commitFoods()
{
  emit foodCollectionAdded(ui.ftFoodsToAdd->getAllFoods());
  emit mealsAdded(ui.ftFoodsToAdd->getAllMeals());
  close();
}
