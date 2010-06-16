#include "add_food.h"
#include "model/food_tree_model.h"
#include <QDebug>

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
}

AddFood::~AddFood()
{
}

void AddFood::commitFoods()
{
  emit foodCollectionAdded(ui.ftFoodsToAdd->getAllFoods());
  emit mealsAdded(ui.ftFoodsToAdd->getAllMeals());
  close();
}
