#include "select_food_control.h"
#include "data/unit.h"
#include "data/food.h"
#include "data/single_food.h"
#include "data/composite_food.h"

SelectFoodControl::SelectFoodControl(QWidget *parent)
    : QWidget(parent)
{
	ui.setupUi(this);

	connect(ui.lstResults, SIGNAL(itemActivated(QListWidgetItem*)),
	        this, SLOT(updateAddControls(QListWidgetItem*)));
}

SelectFoodControl::~SelectFoodControl()
{
}

void SelectFoodControl::clearFoodList()
{
  itemToResult.clear();
  ui.lstResults->clear();
  ui.txtAmount->setEnabled(false);
  ui.cbUnit->setEnabled(false);
  ui.cbMeal->setEnabled(false);
  ui.btnAdd->setEnabled(false);
}

void SelectFoodControl::addToFoodList(const FoodSearchControl::Result& result)
{
  ui.lstResults->addItem(result.description);
  itemToResult.insert(ui.lstResults->count()-1, result);
}

void SelectFoodControl::updateAddControls(QListWidgetItem* curSelectedItem)
{
  QSharedPointer<Food> food;

  if (curSelectedItem != NULL) {
    const FoodSearchControl::Result& result = itemToResult[ui.lstResults->currentIndex().row()];

    if (result.type == "Food") {
      food = SingleFood::getSingleFood(result.id);
    } else {
      food = CompositeFood::getCompositeFood(result.id);
    }
  }

  ui.cbUnit->clear();

  if (food != NULL) {

    QList<Unit::Dimensions::Dimension> validDimensions = food->getValidDimensions();

    for (QList<Unit::Dimensions::Dimension>::const_iterator i = validDimensions.begin();
        i != validDimensions.end(); ++i)
    {

      QVector<QSharedPointer<const Unit> > units = Unit::getAllUnits(*i);

      for (QVector<QSharedPointer<const Unit> >::const_iterator i = units.begin();
          i != units.end(); ++i)
      {
        ui.cbUnit->addItem((*i)->getNameAndAbbreviation());
      }
    }
  }

  bool enableControls = (food != NULL) && (curSelectedItem != NULL);

  ui.txtAmount->setEnabled(enableControls);
  ui.cbUnit->setEnabled(enableControls);
  ui.cbMeal->setEnabled(enableControls);
  ui.btnAdd->setEnabled(enableControls);
}
