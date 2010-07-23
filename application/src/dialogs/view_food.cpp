#include "view_food.h"
#include <QDebug>
#include <QtGui/QDoubleValidator>
#include <QtGui/QSpacerItem>
#include <QtGui/QMessageBox>
#include <QSettings>
#include "libnutrition/data/unit.h"
#include "libnutrition/data/group.h"
#include "model/variant_value_item_model.h"

// The numeric database fields are DECIMAL(10,4), so 10 digits, 4 of which
// appear after the decimal point.
// TODO: Find a more logical place to encode these restrictions
const double ViewFood::MAX_ENTRY = 999999.9999;
const int ViewFood::MAX_DECIMALS = 4;

ViewFood::ViewFood(const FoodAmount& foodAmount, QWidget *parent)
    : QDialog(parent), foodAmount(foodAmount)
{
  ui.setupUi(this);

  // TODO: Add functionality so that the user can change the amount and have
  // the changes reflected in the dialog.

  populateUnitSelector(ui.cboUnit);

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

  connect(ui.btnClose, SIGNAL(clicked()), this, SLOT(close()));

  loadFoodInformation();

  QSettings settings("Nerdland", "Nutrition Tracker");
  settings.beginGroup("viewfood");
  resize(settings.value("size", size()).toSize());
  if (!settings.value("position").isNull()) {
    move(settings.value("position", pos()).toPoint());
  }
  settings.endGroup();
}

ViewFood::~ViewFood()
{
  QSettings settings("Nerdland", "Nutrition Tracker");
  settings.beginGroup("viewfood");
  settings.setValue("size", size());
  settings.setValue("position", pos());
  settings.endGroup();
}

void ViewFood::populateUnitSelector(QComboBox* cboUnit)
{
  QList<Unit::Dimensions::Dimension> dimensions = foodAmount.getFood()->getValidDimensions();

  for (QList<Unit::Dimensions::Dimension>::const_iterator i = dimensions.begin();
      i != dimensions.end(); ++i)

  {
    QVector<QSharedPointer<const Unit> > units = Unit::getAllUnits(*i);

    for (QVector<QSharedPointer<const Unit> >::const_iterator i = units.begin();
        i != units.end(); ++i)
    {
      cboUnit->addItem((*i)->getNameAndAbbreviation(), (*i)->getAbbreviation());
    }
  }

  cboUnit->setCurrentIndex(cboUnit->findData(foodAmount.getUnit()->getAbbreviation()));
}

void ViewFood::populateDimensionSelector(QComboBox* cboDimension)
{
  cboDimension->setModel
    (new VariantValueItemModel<NutrientAmountDisplay::DisplayModes::DisplayMode>(cboDimension));

  cboDimension->addItem
    ("Weight", QVariant::fromValue(NutrientAmountDisplay::DisplayModes::Weight));
  cboDimension->addItem
    ("% RDI", QVariant::fromValue(NutrientAmountDisplay::DisplayModes::RDI));
}

void ViewFood::populateNutrientGroup
  (QGroupBox* grpNutrients, QVector<NutrientAmountDisplay>& amountDisplays,
   Nutrient::Categories::Category category)
{
  QGridLayout& layout = dynamic_cast<QGridLayout&>(*(grpNutrients->layout()));
  QVector<QSharedPointer<const Nutrient> > nutrients = Nutrient::getAllNutrients(category);

  for (QVector<QSharedPointer<const Nutrient> >::iterator i = nutrients.begin();
      i != nutrients.end(); ++i)
  {
    int row = layout.rowCount();

    NutrientAmountDisplay display
      (grpNutrients, *i, 0, NutrientAmountDisplay::DisplayModes::Weight, false);

    layout.addWidget(display.getNameWidget(), row, 1);
    layout.addWidget(display.getValueWidget(), row, 2);
    layout.addWidget(display.getUnitWidget(), row, 3);

    amountDisplays.push_back(display);
  }

  layout.addItem
    (new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding),
     layout.rowCount(), 2);
}

void ViewFood::loadNutrientInformation(QVector<NutrientAmountDisplay>& nutrientDisplays,
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

void ViewFood::loadFoodInformation()
{
  ui.txtName->setText(foodAmount.getFood()->getDisplayName());

  // TODO: Load category name. But currently this only works for single foods, and this
  // might be a composite or a meal

  // TODO: Load source name. But currently this only works for single foods, and this
  // might be a composite or a meal

  QSharedPointer<const User> owner = foodAmount.getFood()->getOwner();

  ui.txtOwner->setText(owner ? owner->getDisplayName() : "");

  ui.txtAmount->setText(QString::number(foodAmount.getAmount()));

  QMap<QString, NutrientAmount> foodNutrients = foodAmount.getScaledNutrients();

  QString caloriesId = Nutrient::getNutrientByName("Calories")->getId();
  ui.txtCalories->setValidator(new QDoubleValidator(0.0, MAX_ENTRY, MAX_DECIMALS, ui.txtCalories));
  if (foodNutrients.contains(caloriesId)) {
    ui.txtCalories->setText(QString::number(foodNutrients[caloriesId].getAmount()));
  }

  loadNutrientInformation(basicNutrients, foodNutrients);
  loadNutrientInformation(vitamins, foodNutrients);
  loadNutrientInformation(minerals, foodNutrients);
}

void ViewFood::changeDisplayModes(QVector<NutrientAmountDisplay>& nutrients,
                                     NutrientAmountDisplay::DisplayModes::DisplayMode mode)
{
  for (QVector<NutrientAmountDisplay>::iterator i = nutrients.begin();
       i != nutrients.end(); ++i)
  {
    i->setDisplayMode(mode);
  }
}

void ViewFood::basicNutrientsDimensionChanged(int newIndex)
{
  changeDisplayModes
    (basicNutrients, ui.cboNutrientDimensions->itemData(newIndex).value
       <NutrientAmountDisplay::DisplayModes::DisplayMode>());
}

void ViewFood::vitaminsDimensionChanged(int newIndex)
{
  changeDisplayModes
    (vitamins, ui.cboVitaminDimensions->itemData(newIndex).value
       <NutrientAmountDisplay::DisplayModes::DisplayMode>());
}

void ViewFood::mineralsDimensionChanged(int newIndex)
{
  changeDisplayModes
    (minerals, ui.cboMineralDimensions->itemData(newIndex).value
       <NutrientAmountDisplay::DisplayModes::DisplayMode>());
}
