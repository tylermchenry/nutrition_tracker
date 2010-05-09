#include "add_food.h"

AddFood::AddFood(const QSqlDatabase& db, QWidget *parent)
    : QDialog(parent), db(db)
{
	ui.setupUi(this);
	ui.fscSearch->setDatabase(db);

	connect(ui.fscSearch, SIGNAL(beginNewSearch()), ui.lstResults, SLOT(clear()));
	connect(ui.fscSearch, SIGNAL(newResult(int, const QString&, const QString&)),
	        this, SLOT(addSearchResult(int, const QString&, const QString&)));
}

AddFood::~AddFood()
{
}

void AddFood::addSearchResult(int id, const QString& type, const QString& description)
{
  ui.lstResults->addItem(description);
}
