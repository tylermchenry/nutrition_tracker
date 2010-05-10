#include "add_food.h"

AddFood::AddFood(const QSqlDatabase& db, QWidget *parent)
    : QDialog(parent), db(db)
{
	ui.setupUi(this);
	ui.fscSearch->setDatabase(db);

	connect(ui.fscSearch, SIGNAL(beginNewSearch()),
	        ui.sfcResultsList, SLOT(clearFoodList()));
	connect(ui.fscSearch, SIGNAL(newResult(int, const QString&, const QString&)),
	        ui.sfcResultsList, SLOT(addToFoodList(int, const QString&, const QString&)));
}

AddFood::~AddFood()
{
}
