#include "edit_composite_food.h"
#include "data/group.h"
#include "data/unit.h"
#include <QtGui/QMessageBox>
#include <QDebug>

// The numeric database fields are DECIMAL(10,4), so 10 digits, 4 of which
// appear after the decimal point.
// TODO: Find a more logical place to encode these restrictions
const double EditCompositeFood::MAX_ENTRY = 999999.9999;
const int EditCompositeFood::MAX_DECIMALS = 4;

EditCompositeFood::EditCompositeFood(const QSqlDatabase& db, QWidget *parent,
                                          const QSharedPointer<CompositeFood>& food)
    : QDialog(parent), db(db), food(food)
{
	ui.setupUi(this);

    populateUserSelector(ui.cboOwner);

    // TODO: Connect owner combobox when it is enabled

    populateUnitSelector(ui.cboWeightUnit, Unit::Dimensions::Weight);
    populateUnitSelector(ui.cboVolumeUnit, Unit::Dimensions::Volume);

	ui.fscSearch->setDatabase(db);

	ui.sfcResultsList->setAllowMealSelection(false);

	connect(ui.fscSearch, SIGNAL(beginNewSearch()),
	        ui.sfcResultsList, SLOT(clearFoodList()));
	connect(ui.fscSearch, SIGNAL(newResult(const FoodSearchControl::Result&)),
	        ui.sfcResultsList, SLOT(addToFoodList(const FoodSearchControl::Result&)));

	connect(ui.sfcResultsList, SIGNAL(amountAdded(const FoodAmount&, int)),
	        ui.ftComponents, SLOT(addFoodAmount(const FoodAmount&)));

	connect(ui.btnCancel, SIGNAL(clicked()), this, SLOT(close()));
	connect(ui.btnSave, SIGNAL(clicked()), this, SLOT(saveFood()));
	connect(ui.btnSaveAndAdd, SIGNAL(clicked()), this, SLOT(saveFoodAndClear()));
	connect(ui.btnSaveAndClose, SIGNAL(clicked()), this, SLOT(saveFoodAndClose()));

	ui.ftComponents->setTemporary(true);
	ui.ftComponents->setRootName("Composite Food");
	ui.ftComponents->initialize();

	loadFoodInformation();
}

EditCompositeFood::~EditCompositeFood()
{
}

void EditCompositeFood::clearFood()
{
  food.clear();
  loadFoodInformation();
}

bool EditCompositeFood::saveFood()
{
  try {

    qDebug() << "Saving composite food.";

    food->setName(ui.txtName->text());

    food->setBaseAmount
    (ui.txtWeight->text().toDouble(),
     Unit::getUnit(ui.cboWeightUnit->itemData(ui.cboWeightUnit->currentIndex()).toString()));

    food->setBaseAmount
    (ui.txtVolume->text().toDouble(),
     Unit::getUnit(ui.cboVolumeUnit->itemData(ui.cboVolumeUnit->currentIndex()).toString()));

    food->setBaseAmount
    (ui.txtQuantity->text().toDouble(), Unit::getPreferredUnit(Unit::Dimensions::Quantity));

    food->setBaseAmount
    (ui.txtServings->text().toDouble(), Unit::getPreferredUnit(Unit::Dimensions::Serving));

    food->saveToDatabase();

    return true;

  } catch (std::exception& ex) {
    QMessageBox::critical(this, "Failed to save",
                          QString("Unable to save composite food. Error was: ") + ex.what());
    return false;
  }
}

void EditCompositeFood::populateUserSelector(QComboBox* cboOwner)
{
  cboOwner->addItem("User");
}

void EditCompositeFood::populateUnitSelector(QComboBox* cboUnit, Unit::Dimensions::Dimension dimension)
{
  QVector<QSharedPointer<const Unit> > units = Unit::getAllUnits(dimension);

  for (QVector<QSharedPointer<const Unit> >::const_iterator i = units.begin();
      i != units.end(); ++i)
  {
    cboUnit->addItem((*i)->getNameAndAbbreviation(), (*i)->getAbbreviation());
  }

  cboUnit->setCurrentIndex
    (cboUnit->findData(Unit::getPreferredUnit(dimension)->getAbbreviation()));
}

void EditCompositeFood::loadAmountInformation(QLineEdit* txtAmount, QComboBox* cboUnit,
                                                    Unit::Dimensions::Dimension dimension)
{
  FoodAmount baseAmount = food->getBaseAmount(dimension);

  txtAmount->setValidator(new QDoubleValidator(0.0, MAX_ENTRY, MAX_DECIMALS, txtAmount));

  if (baseAmount.isDefined()) {

    if (txtAmount != NULL) {
      txtAmount->setText(QString::number(baseAmount.getAmount()));
    }

    if (cboUnit != NULL) {
      cboUnit->setCurrentIndex
      (cboUnit->findData(baseAmount.getUnit()->getAbbreviation()));
    }
  }
}

void EditCompositeFood::loadFoodInformation()
{
  if (food == NULL) {
    food = CompositeFood::createNewCompositeFood();
  } else {
    ui.ftComponents->setRootCollection(food);
  }

  ui.ftComponents->clear();

  ui.txtName->setText(food->getName());

  // TODO: Set user when users are supported

  loadAmountInformation(ui.txtWeight, ui.cboWeightUnit, Unit::Dimensions::Weight);
  loadAmountInformation(ui.txtVolume, ui.cboVolumeUnit, Unit::Dimensions::Volume);
  loadAmountInformation(ui.txtQuantity, NULL, Unit::Dimensions::Quantity);
  loadAmountInformation(ui.txtServings, NULL, Unit::Dimensions::Serving);
}

void EditCompositeFood::saveFoodAndClose()
{
  if (saveFood()) {
    close();
  }
}

void EditCompositeFood::saveFoodAndClear()
{
  if (saveFood()) {
    clearFood();
  }
}
