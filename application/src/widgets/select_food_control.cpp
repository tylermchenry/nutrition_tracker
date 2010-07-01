#include "select_food_control.h"
#include "libnutrition/data/unit.h"
#include "libnutrition/data/single_food.h"
#include "libnutrition/data/composite_food.h"
#include "libnutrition/data/meal.h"
#include <QDebug>
#include <QSettings>

SelectFoodControl::SelectFoodControl(QWidget *parent)
  : QWidget(parent), selectMeals(true), specializedSeparatorRow(-1)
{
  initialize();
}

SelectFoodControl::SelectFoodControl(bool selectMeals, QWidget *parent)
  : QWidget(parent), selectMeals(selectMeals), specializedSeparatorRow(-1)
{
  initialize();
}

SelectFoodControl::~SelectFoodControl()
{
}

void SelectFoodControl::setAllowMealSelection(bool selectMeals)
{
  this->selectMeals = selectMeals;

  ui.lblMeal->setVisible(selectMeals);
  ui.cbMeal->setVisible(selectMeals);
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

void SelectFoodControl::updateAddControls()
{
  QListWidgetItem* curSelectedItem = ui.lstResults->currentItem();

  if (curSelectedItem != NULL) {

    const FoodSearchControl::Result& result = itemToResult[ui.lstResults->currentIndex().row()];

    if (result.type == "Food") {
      selectedFood = SingleFood::getSingleFood(result.id);
    } else {
      selectedFood = CompositeFood::getCompositeFood(result.id);
    }

  } else {
    selectedFood.clear();
  }

  ui.cbUnit->clear();

  if (selectedFood != NULL) {
    QList<Unit::Dimensions::Dimension> validDimensions = selectedFood->getValidDimensions();

    for (QList<Unit::Dimensions::Dimension>::const_iterator i = validDimensions.begin();
        i != validDimensions.end(); ++i)
    {

      QVector<QSharedPointer<const Unit> > units = Unit::getAllUnits(*i);

      for (QVector<QSharedPointer<const Unit> >::const_iterator i = units.begin();
          i != units.end(); ++i)
      {
        ui.cbUnit->addItem((*i)->getNameAndAbbreviation(), (*i)->getAbbreviation());
      }
    }

    QVector<QSharedPointer<const SpecializedUnit> > specializedUnits =
      selectedFood->getAllSpecializedUnits();

    if (!specializedUnits.isEmpty()) {

      specializedSeparatorRow = ui.cbUnit->count();
      ui.cbUnit->insertSeparator(specializedSeparatorRow);

      for (QVector<QSharedPointer<const SpecializedUnit> >::const_iterator i = specializedUnits.begin();
           i != specializedUnits.end(); ++i)
      {
        ui.cbUnit->addItem((*i)->getNameAndAbbreviation(), (*i)->getSequence());
      }

    } else {
      specializedSeparatorRow = -1;
    }

  }

  bool enableControls = (selectedFood != NULL) && (curSelectedItem != NULL);

  ui.txtAmount->setEnabled(enableControls);
  ui.cbUnit->setEnabled(enableControls);
  ui.cbMeal->setEnabled(enableControls);
  ui.btnAdd->setEnabled(enableControls);

  bool enableRefuse = enableControls && (selectedFood->getPercentRefuse() > 0);

  ui.chkIncludeRefuse->setChecked(enableRefuse && includeRefuse);
  ui.chkIncludeRefuse->setEnabled(enableRefuse);
  ui.lblRefuse->setVisible(enableRefuse);
  ui.lblRefuseDescription->setVisible(enableRefuse);

  if (enableRefuse) {
    ui.lblRefuseDescription->setText(selectedFood->getRefuseDescription());
  }

}

void SelectFoodControl::addClicked()
{
  qDebug() << "Add clicked";
  if (selectedFood != NULL) {
    qDebug() << "Emitting amountAdded for " << selectedFood->getName();

    double amount;
    QSharedPointer<const Unit> unit;

    if (specializedSeparatorRow < 0 || ui.cbUnit->currentIndex() < specializedSeparatorRow) {
      amount = ui.txtAmount->text().toDouble();
      unit = Unit::getUnit(ui.cbUnit->itemData(ui.cbUnit->currentIndex()).toString());

    } else {
      QSharedPointer<const SpecializedUnit> specializedUnit =
        selectedFood->getSpecializedUnit
          (ui.cbUnit->itemData(ui.cbUnit->currentIndex()).toInt());

      FoodAmount baseAmount = specializedUnit->getBaseAmount();

      amount = baseAmount.getAmount() * ui.txtAmount->text().toDouble();
      unit = baseAmount.getUnit();
    }

    emit amountAdded
      (FoodAmount(selectedFood, amount, unit,
                  !ui.chkIncludeRefuse->isEnabled() || ui.chkIncludeRefuse->isChecked()),
                  (selectMeals ? ui.cbMeal->itemData(ui.cbMeal->currentIndex()).toInt() : -1));
  }
}

void SelectFoodControl::initialize()
{
  ui.setupUi(this);

  if (!selectMeals) {
    ui.lblMeal->setVisible(false);
    ui.cbMeal->setVisible(false);
  } else {
    populateMealSelector(ui.cbMeal);
  }

  ui.lblRefuse->setVisible(false);
  ui.lblRefuseDescription->setVisible(false);

  connect(ui.lstResults, SIGNAL(itemSelectionChanged()),
          this, SLOT(updateAddControls()));
  connect(ui.btnAdd, SIGNAL(clicked()), this, SLOT(addClicked()));

  QSettings settings("Nerdland", "Nutrition Tracker");

  settings.beginGroup("preferences");
  includeRefuse = settings.value("includerefuse", true).toBool();
  settings.endGroup();
}

void SelectFoodControl::populateMealSelector(QComboBox* cbMeals)
{
  qDebug() << "Populating meal selector...";
  QMap<int, QString> mealNames = Meal::getAllMealNames();

  for (QMap<int, QString>::const_iterator i = mealNames.begin(); i != mealNames.end(); ++i)
  {
    cbMeals->addItem(i.value(), i.key());
  }
}
