#include "add_food.h"

AddFood::AddFood(const QSqlDatabase& db, QWidget *parent)
    : QDialog(parent), db(db)
{
	ui.setupUi(this);
	ui.fscSearch->setDatabase(db);

	connect(ui.fscSearch, SIGNAL(beginNewSearch()), this, SLOT(clearSearchResults()));
	connect(ui.fscSearch, SIGNAL(newResult(int, const QString&, const QString&)),
	        this, SLOT(addSearchResult(int, const QString&, const QString&)));
	connect(ui.lstResults, SIGNAL(currentItemChanged(QListWidgetItem*, QListWidgetItem*)),
	        this, SLOT(updateAddControls(QListWidgetItem*)));
}

AddFood::~AddFood()
{
}

void AddFood::clearSearchResults()
{
  ui.lstResults->clear();
  ui.dsbAmount->setEnabled(false);
  ui.cbUnit->setEnabled(false);
  ui.cbMeal->setEnabled(false);
  ui.btnAdd->setEnabled(false);
}

void AddFood::addSearchResult(int id, const QString& type, const QString& description)
{
  ui.lstResults->addItem(description);
}

void AddFood::updateAddControls(QListWidgetItem* curSelectedItem)
{
  bool enableControls = (curSelectedItem != NULL);

  ui.dsbAmount->setEnabled(enableControls);
  ui.cbUnit->setEnabled(enableControls);
  ui.cbMeal->setEnabled(enableControls);
  ui.btnAdd->setEnabled(enableControls);
}
