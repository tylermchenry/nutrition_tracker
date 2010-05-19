#include "edit_food.h"
#include <QDebug>
#include <QtGui/QSpacerItem>
#include "data/unit.h"
#include "data/group.h"
#include <cassert>

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
    // TODO: De-hardcode this once I can figure out a way to have QComboBox::findData work
    // with a QVariant containing a DisplayMode enumeration.
    ui.cboVitaminDimensions->setCurrentIndex(1);
    ui.cboMineralDimensions->setCurrentIndex(1);

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

  loadAmountInformation(ui.txtWeight, ui.cboWeightUnit, Unit::Dimensions::Weight);
  loadAmountInformation(ui.txtVolume, ui.cboVolumeUnit, Unit::Dimensions::Volume);
  loadAmountInformation(ui.txtQuantity, NULL, Unit::Dimensions::Quantity);
  loadAmountInformation(ui.txtServings, NULL, Unit::Dimensions::Serving);

  QMap<QString, NutrientAmount> foodNutrients = food->getNutrients();

  QString caloriesId = Nutrient::getNutrientByName("Calories")->getId();
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

EditFood::NutrientAmountDisplay::NutrientAmountDisplay
  (QWidget* widgetParent, const QSharedPointer<const Nutrient>& nutrient,
   double amount, DisplayModes::DisplayMode mode)
  : nutrientAmount(nutrient, amount), displayMode(mode), supportsRDI(false)
{
  initialize(widgetParent);
}

EditFood::NutrientAmountDisplay::NutrientAmountDisplay
  (QWidget* widgetParent, const NutrientAmount& amount,
   DisplayModes::DisplayMode mode)
  : nutrientAmount(amount), displayMode(mode), supportsRDI(false)
{
  initialize(widgetParent);
}

void EditFood::NutrientAmountDisplay::setDisplayMode(DisplayModes::DisplayMode mode)
{
  setDisplayMode(mode, false);
}

void EditFood::NutrientAmountDisplay::initialize(QWidget* widgetParent)
{
  QString name;

  if (nutrientAmount.getNutrient() != NULL) {
    name = nutrientAmount.getNutrient()->getName();
    supportsRDI = (nutrientAmount.getNutrient()->getRDI() > 0);
  }

  lblName = new QLabel(name, widgetParent);
  txtValue = new QLineEdit(widgetParent);
  lblUnit = new QLabel(widgetParent);

  lblName->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
  txtValue->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
  lblUnit->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);

  setDisplayMode(displayMode, true);
}

void EditFood::NutrientAmountDisplay::setNutrientAmount(const NutrientAmount& newAmount)
{
  nutrientAmount = newAmount;

  if (nutrientAmount.getNutrient() != NULL) {
    lblName->setText(nutrientAmount.getNutrient()->getName());
    supportsRDI = (nutrientAmount.getNutrient()->getRDI() > 0);
  } else {
    lblName->setText("");
    supportsRDI = false;
  }

  setDisplayMode(displayMode, true);
}

void EditFood::NutrientAmountDisplay::setDisplayMode(DisplayModes::DisplayMode mode, bool force)
{
  if (mode != displayMode || force) {

    switch (mode) {

      case DisplayModes::Weight:

        if (!supportsRDI) {
          txtValue->setEnabled(true);
        }

        txtValue->setText(QString::number(nutrientAmount.getAmount()));

        if (nutrientAmount.getNutrient() != NULL) {
          lblUnit->setText(nutrientAmount.getUnit()->getAbbreviation());
        } else {
          lblUnit->setText("");
        }

        break;

      case DisplayModes::RDI:

        if (supportsRDI) {
          txtValue->setText(QString::number(nutrientAmount.getAmountAsPercentRDI() * 100));
          lblUnit->setText("% RDI");
        } else {
          txtValue->setEnabled(false);
        }

        break;
    }

    if (txtValue->text().toDouble() == 0) {
      txtValue->setText("");
    }

    displayMode = mode;
  }
}


