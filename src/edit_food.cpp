#include "edit_food.h"
#include "data/unit.h"
#include <QDebug>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlRecord>
#include <QtSql/QSqlError>
#include <QtGui/QSpacerItem>

EditFood::EditFood(const QSqlDatabase& db, QWidget *parent)
    : QDialog(parent), db(db)
{
	ui.setupUi(this);

	ui.cboNutrientDimensions->addItem("Weight", NutrientAmountDisplay::DisplayModes::Weight);
	ui.cboNutrientDimensions->addItem("% RDI", NutrientAmountDisplay::DisplayModes::RDI);
	connect(ui.cboNutrientDimensions, SIGNAL(currentIndexChanged(int)),
	        this, SLOT(basicNutrientsDimensionChanged(int)));

	ui.cboVitaminDimensions->addItem("Weight", NutrientAmountDisplay::DisplayModes::Weight);
    ui.cboVitaminDimensions->addItem("% RDI", NutrientAmountDisplay::DisplayModes::RDI);

    ui.cboMineralDimensions->addItem("Weight", NutrientAmountDisplay::DisplayModes::Weight);
    ui.cboMineralDimensions->addItem("% RDI", NutrientAmountDisplay::DisplayModes::RDI);

    ui.cboSource->addItem("Custom");
    ui.cboSource->addItem("Import");
    ui.cboSource->addItem("USDA");

    ui.cboOwner->addItem("User");

    QSqlQuery query(db);

    QVector<QSharedPointer<const Unit> > weightUnits =
        Unit::getAllUnits(Unit::Dimensions::Weight);

    for (QVector<QSharedPointer<const Unit> >::const_iterator i = weightUnits.begin();
         i != weightUnits.end(); ++i)
    {
      ui.cboWeightUnit->addItem((*i)->getNameAndAbbreviation());
      if (**i == *Unit::getPreferredUnit(Unit::Dimensions::Weight)) {
          ui.cboWeightUnit->setCurrentIndex(ui.cboWeightUnit->count()-1);
      }
    }

    QVector<QSharedPointer<const Unit> > volumeUnits =
       Unit::getAllUnits(Unit::Dimensions::Volume);

    for (QVector<QSharedPointer<const Unit> >::const_iterator i = volumeUnits.begin();
         i != volumeUnits.end(); ++i)
    {
      ui.cboVolumeUnit->addItem((*i)->getNameAndAbbreviation());
      if (**i == *Unit::getPreferredUnit(Unit::Dimensions::Volume)) {
          ui.cboVolumeUnit->setCurrentIndex(ui.cboVolumeUnit->count()-1);
      }
    }

    if (query.exec("SELECT FdGrp_Cd, FdGrp_Desc FROM group_description ORDER BY FdGrp_Desc ASC")) {
      while (query.next()) {
        ui.cboCategory->addItem(query.value(1).toString(), query.value(0).toString());
        if (query.value(0).toString() == "2200") { /* Meals, Entrees and Sidedishes */
           ui.cboCategory->setCurrentIndex(ui.cboCategory->count()-1);
        }
      }
    }

    QGridLayout& basicNutrientsLayout =
        dynamic_cast<QGridLayout&>(*(ui.grpBasicNutrients->layout()));

    QVector<QSharedPointer<const Nutrient> > allBasicNutrients =
        Nutrient::getAllNutrients(Nutrient::Categories::Basic);

    for (QVector<QSharedPointer<const Nutrient> >::iterator i = allBasicNutrients.begin();
          i != allBasicNutrients.end(); ++i)
    {
      int row = basicNutrientsLayout.rowCount();

      NutrientAmountDisplay display(ui.grpBasicNutrients, *i, 0);

      basicNutrientsLayout.addWidget(display.getNameWidget(), row, 1);
      basicNutrientsLayout.addWidget(display.getValueWidget(), row, 2);
      basicNutrientsLayout.addWidget(display.getUnitWidget(), row, 3);

      basicNutrients.push_back(display);
    }

    basicNutrientsLayout.addItem
      (new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding),
       basicNutrientsLayout.rowCount(), 2);

    QGridLayout& vitaminsLayout =
        dynamic_cast<QGridLayout&>(*(ui.grpVitamins->layout()));

    QVector<QSharedPointer<const Nutrient> > allVitamins =
        Nutrient::getAllNutrients(Nutrient::Categories::Vitamin);

    for (QVector<QSharedPointer<const Nutrient> >::iterator i = allVitamins.begin();
          i != allVitamins.end(); ++i)
    {
      int row = vitaminsLayout.rowCount();

      NutrientAmountDisplay display(ui.grpVitamins, *i, 0);

      vitaminsLayout.addWidget(display.getNameWidget(), row, 1);
      vitaminsLayout.addWidget(display.getValueWidget(), row, 2);
      vitaminsLayout.addWidget(display.getUnitWidget(), row, 3);

      vitamins.push_back(display);
    }

    vitaminsLayout.addItem
      (new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding),
       vitaminsLayout.rowCount(), 2);

    QGridLayout& mineralsLayout =
        dynamic_cast<QGridLayout&>(*(ui.grpMinerals->layout()));

    QVector<QSharedPointer<const Nutrient> > allMinerals =
        Nutrient::getAllNutrients(Nutrient::Categories::Mineral);

    for (QVector<QSharedPointer<const Nutrient> >::iterator i = allMinerals.begin();
          i != allMinerals.end(); ++i)
    {
      int row = mineralsLayout.rowCount();

      NutrientAmountDisplay display(ui.grpMinerals, *i, 0);

      mineralsLayout.addWidget(display.getNameWidget(), row, 1);
      mineralsLayout.addWidget(display.getValueWidget(), row, 2);
      mineralsLayout.addWidget(display.getUnitWidget(), row, 3);

      minerals.push_back(display);
    }

    mineralsLayout.addItem
      (new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding),
       mineralsLayout.rowCount(), 2);

}

EditFood::~EditFood()
{

}

void EditFood::changeDisplayModes(QVector<NutrientAmountDisplay>& nutrients,
                                     NutrientAmountDisplay::DisplayModes::DisplayMode mode)
{
  qDebug() << "Changing display mode to " <<
              (mode == NutrientAmountDisplay::DisplayModes::Weight ? "Weight" : "% RDI");
  for (QVector<NutrientAmountDisplay>::iterator i = nutrients.begin();
       i != nutrients.end(); ++i)
  {
    i->setDisplayMode(mode);
  }
}

void EditFood::basicNutrientsDimensionChanged(int newIndex)
{
  qDebug() << "Can convert? "
            << ui.cboNutrientDimensions->itemData(newIndex).canConvert
                <NutrientAmountDisplay::DisplayModes::DisplayMode>();
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

void EditFood::NutrientAmountDisplay::setDisplayMode(DisplayModes::DisplayMode mode, bool force)
{
  if (mode != displayMode || force) {

    switch (mode) {

      case DisplayModes::Weight:

        if (!supportsRDI) {
          txtValue->setEnabled(true);
        }

        txtValue->setText(QString::number(nutrientAmount.getAmount()));
        lblUnit->setText(nutrientAmount.getUnit()->getAbbreviation());

        break;

      case DisplayModes::RDI:

        if (supportsRDI) {
          txtValue->setText(QString::number(nutrientAmount.getAmountAsPercentRDI()));
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


