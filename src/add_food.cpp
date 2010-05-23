#include "add_food.h"
#include "model/food_tree_model.h"

AddFood::AddFood(const QSqlDatabase& db, QWidget *parent)
    : QDialog(parent), db(db)
{
	ui.setupUi(this);
	ui.fscSearch->setDatabase(db);

	FoodTreeModel* foodTreeModel = new FoodTreeModel(ui.trvFoodsToAdd);

	connect(ui.fscSearch, SIGNAL(beginNewSearch()),
	        ui.sfcResultsList, SLOT(clearFoodList()));
	connect(ui.fscSearch, SIGNAL(newResult(const FoodSearchControl::Result&)),
	        ui.sfcResultsList, SLOT(addToFoodList(const FoodSearchControl::Result&)));
	connect(ui.sfcResultsList, SIGNAL(amountAdded(const FoodAmount&, int)),
	        foodTreeModel, SLOT(addFoodAmount(const FoodAmount&, int)));

	connect(foodTreeModel, SIGNAL(newGroupingCreated(const QModelIndex&)),
	        this, SLOT(expandGrouping(const QModelIndex&)));

	ui.trvFoodsToAdd->setModel(foodTreeModel);

	ui.trvFoodsToAdd->header()->setResizeMode(QHeaderView::ResizeToContents);
}

AddFood::~AddFood()
{
}

void AddFood::expandGrouping(const QModelIndex& index)
{
  ui.trvFoodsToAdd->setExpanded(index, true);
}
