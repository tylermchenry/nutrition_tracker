#include "edit_food.h"
#include <QDebug>
#include <QtGui/QDoubleValidator>
#include <QtGui/QSpacerItem>
#include "data/unit.h"
#include "data/group.h"
#include "model/variant_value_item_model.h"
#include <cassert>

// The numeric database fields are DECIMAL(10,4), so 10 digits, 4 of which
// appear after the decimal point.
// TODO: Find a more logical place to encode these restrictions
const double EditFood::MAX_ENTRY = 999999.9999;
const int EditFood::MAX_DECIMALS = 4;

EditFood::EditFood(QWidget *parent, const QSharedPointer<SingleFood>& food)
    : QDialog(parent), food(food)
{
	ui.setupUi(this);

    populateGroupSelector(ui.cboCategory);
    populateSourceSelector(ui.cboSource);
    populateUserSelector(ui.cboOwner);

    populateUnitSelector(ui.cboWeightUnit, Unit::Dimensions::Weight);
    populateUnitSelector(ui.cboVolumeUnit, Unit::Dimensions::Volume);

	populateDimensionSelector(ui.cboNutrientDimensions);
	connect(ui.cboNutrientDimensions, SIGNAL(currentIndexChanged(int)),
	        this, SLOT(basicNutrientsDimensionChanged(int)));

	populateDimensionSelector(ui.cboVitaminDimensions);
    connect(ui.cboVitaminDimensions, SIGNAL(currentIndexChanged(int)),
             this, SLOT(vitaminsDimensionChanged(int)));

    populateDimensionSelector(ui.cboMineralDimensions);
    connect(ui.cboMineralDimensions, SIGNAL(currentIndexChanged(int)),
             this, SLOT(mineralsDimensionChanged(int)));

    populateNutrientGroup(ui.grpBasicNutrients, basicNutrients, Nutrient::Categories::Basic);
    populateNutrientGroup(ui.grpVitamins, vitamins, Nutrient::Categories::Vitamin);
    populateNutrientGroup(ui.grpMinerals, minerals, Nutrient::Categories::Mineral);

    // Default Vitamin and Mineral dimensions to % RDI

    ui.cboVitaminDimensions->setCurrentIndex
      (ui.cboVitaminDimensions->findData
         (QVariant::fromValue(NutrientAmountDisplay::DisplayModes::RDI)));

    ui.cboMineralDimensions->setCurrentIndex
      (ui.cboMineralDimensions->findData
         (QVariant::fromValue(NutrientAmountDisplay::DisplayModes::RDI)));

    if (food != NULL) loadFoodInformation();
}

EditFood::~EditFood()
{

}

void EditFood::populateSourceSelector(QComboBox* cboSource)
{
  cboSource->addItem("Custom");
  cboSource->addItem("Import");
  cboSource->addItem("USDA");
}

void EditFood::populateUserSelector(QComboBox* cboOwner)
{
  cboOwner->addItem("User");
}

void EditFood::populateGroupSelector(QComboBox* cboGroup)
{
  QVector<QSharedPointer<const Group> > groups = Group::getAllGroups();

  for (QVector<QSharedPointer<const Group> >::const_iterator i = groups.begin();
      i != groups.end(); ++i)
  {
    cboGroup->addItem((*i)->getName(), (*i)->getId());
  }

  cboGroup->setCurrentIndex
    (cboGroup->findData(Group::getDefaultGroup()->getId()));
}

void EditFood::populateUnitSelector(QComboBox* cboUnit, Unit::Dimensions::Dimension dimension)
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

void EditFood::populateDimensionSelector(QComboBox* cboDimension)
{
  cboDimension->setModel
    (new VariantValueItemModel<NutrientAmountDisplay::DisplayModes::DisplayMode>(cboDimension));

  cboDimension->addItem
    ("Weight", QVariant::fromValue(NutrientAmountDisplay::DisplayModes::Weight));
  cboDimension->addItem
    ("% RDI", QVariant::fromValue(NutrientAmountDisplay::DisplayModes::RDI));
}

void EditFood::populateNutrientGroup
  (QGroupBox* grpNutrients, QVector<NutrientAmountDisplay>& amountDisplays,
   Nutrient::Categories::Category category)
{
  QGridLayout& layout = dynamic_cast<QGridLayout&>(*(grpNutrients->layout()));
  QVector<QSharedPointer<const Nutrient> > nutrients = Nutrient::getAllNutrients(category);

  for (QVector<QSharedPointer<const Nutrient> >::iterator i = nutrients.begin();
      i != nutrients.end(); ++i)
  {
    int row = layout.rowCount();

    NutrientAmountDisplay display(grpNutrients, *i, 0);

    layout.addWidget(display.getNameWidget(), row, 1);
    layout.addWidget(display.getValueWidget(), row, 2);
    layout.addWidget(display.getUnitWidget(), row, 3);

    amountDisplays.push_back(display);
  }

  layout.addItem
    (new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding),
     layout.rowCount(), 2);
}

void EditFood::loadAmountInformation(QLineEdit* txtAmount, QComboBox* cboUnit,
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

void EditFood::loadNutrientInformation(QVector<NutrientAmountDisplay>& nutrientDisplays,
                                             const QMap<QString, NutrientAmount>& nutrients)
{
  for (QVector<NutrientAmountDisplay>::iterator i = nutrientDisplays.begin();
      i != nutrientDisplays.end(); ++i)
  {
    QString nutrientId = i->getNutrientAmount().getNutrient()->getId();
    if (nutrients.contains(nutrientId)) {
      i->setNutrientAmount(nutrients[nutrientId]);
    }
  }
}

void EditFood::loadFoodInformation()
{
  ui.txtName->setText(food->getName());

  ui.cboCategory->setCurrentIndex(ui.cboCategory->findData(food->getGroup()->getId()));

  ui.cboSource->setCurrentIndex
    (ui.cboSource->findText
      (SingleFood::EntrySources::toHumanReadable(food->getEntrySource())));

  // TODO: Set user when users are supported

  loadAmountInformation(ui.txtWeight, ui.cboWeightUnit, Unit::Dimensions::Weight);
  loadAmountInformation(ui.txtVolume, ui.cboVolumeUnit, Unit::Dimensions::Volume);
  loadAmountInformation(ui.txtQuantity, NULL, Unit::Dimensions::Quantity);
  loadAmountInformation(ui.txtServings, NULL, Unit::Dimensions::Serving);

  QMap<QString, NutrientAmount> foodNutrients = food->getNutrients();

  QString caloriesId = Nutrient::getNutrientByName("Calories")->getId();
  ui.txtCalories->setValidator(new QDoubleValidator(0.0, MAX_ENTRY, MAX_DECIMALS, ui.txtCalories));
  if (foodNutrients.contains(caloriesId)) {
    ui.txtCalories->setText(QString::number(foodNutrients[caloriesId].getAmount()));
  }

  loadNutrientInformation(basicNutrients, foodNutrients);
  loadNutrientInformation(vitamins, foodNutrients);
  loadNutrientInformation(minerals, foodNutrients);
}

void EditFood::changeDisplayModes(QVector<NutrientAmountDisplay>& nutrients,
                                     NutrientAmountDisplay::DisplayModes::DisplayMode mode)
{
  for (QVector<NutrientAmountDisplay>::iterator i = nutrients.begin();
       i != nutrients.end(); ++i)
  {
    i->setDisplayMode(mode);
  }
}

void EditFood::basicNutrientsDimensionChanged(int newIndex)
{
  changeDisplayModes
    (basicNutrients, ui.cboNutrientDimensions->itemData(newIndex).value
       <NutrientAmountDisplay::DisplayModes::DisplayMode>());
}

void EditFood::vitaminsDimensionChanged(int newIndex)
{
  changeDisplayModes
    (vitamins, ui.cboVitaminDimensions->itemData(newIndex).value
       <NutrientAmountDisplay::DisplayModes::DisplayMode>());
}

void EditFood::mineralsDimensionChanged(int newIndex)
{
  changeDisplayModes
    (minerals, ui.cboMineralDimensions->itemData(newIndex).value
       <NutrientAmountDisplay::DisplayModes::DisplayMode>());
}



