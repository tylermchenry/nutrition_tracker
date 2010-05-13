#include "select_food_control.h"

SelectFoodControl::SelectFoodControl(QWidget *parent)
    : QWidget(parent)
{
	ui.setupUi(this);

	connect(ui.lstResults, SIGNAL(currentItemChanged(QListWidgetItem*, QListWidgetItem*)),
	        this, SLOT(updateAddControls(QListWidgetItem*)));
}

SelectFoodControl::~SelectFoodControl()
{
}

void SelectFoodControl::clearFoodList()
{
  ui.lstResults->clear();
  ui.txtAmount->setEnabled(false);
  ui.cbUnit->setEnabled(false);
  ui.cbMeal->setEnabled(false);
  ui.btnAdd->setEnabled(false);
}

void SelectFoodControl::addToFoodList(int id, const QString& type, const QString& description)
{
  ui.lstResults->addItem(description);
}

void SelectFoodControl::updateAddControls(QListWidgetItem* curSelectedItem)
{
  bool enableControls = (curSelectedItem != NULL);

  ui.txtAmount->setEnabled(enableControls);
  ui.cbUnit->setEnabled(enableControls);
  ui.cbMeal->setEnabled(enableControls);
  ui.btnAdd->setEnabled(enableControls);
}
