#include "add_food.h"

AddFood::AddFood(const QSqlDatabase& db, QWidget *parent)
    : QDialog(parent), db(db)
{
	ui.setupUi(this);
	ui.fscSearch->setDatabase(db);

	connect(ui.fscSearch, SIGNAL(beginNewSearch()),
	        ui.sfcResultsList, SLOT(clearFoodList()));
	connect(ui.fscSearch, SIGNAL(newResult(const FoodSearchControl::Result&)),
	        ui.sfcResultsList, SLOT(addToFoodList(const FoodSearchControl::Result&)));
}

AddFood::~AddFood()
{
}
