#include "add_food.h"
#include "model/food_tree_model.h"

AddFood::AddFood(const QSqlDatabase& db, QWidget *parent)
    : QDialog(parent), db(db)
{
	ui.setupUi(this);
	ui.fscSearch->setDatabase(db);

	FoodTreeModel* foodTreeModel = new FoodTreeModel();

	connect(ui.fscSearch, SIGNAL(beginNewSearch()),
	        ui.sfcResultsList, SLOT(clearFoodList()));
	connect(ui.fscSearch, SIGNAL(newResult(const FoodSearchControl::Result&)),
	        ui.sfcResultsList, SLOT(addToFoodList(const FoodSearchControl::Result&)));
	connect(ui.sfcResultsList, SIGNAL(amountAdded(const FoodAmount&, int)),
	        foodTreeModel, SLOT(addFoodAmount(const FoodAmount&, int)));

	ui.trvFoodsToAdd->setModel(foodTreeModel);

	ui.trvFoodsToAdd->header()->setResizeMode(QHeaderView::ResizeToContents);
}

AddFood::~AddFood()
{
}
