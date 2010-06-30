/*
 * nutrient_amount_display.cpp
 *
 *  Created on: May 20, 2010
 *      Author: tyler
 */

#include "nutrient_amount_display.h"
#include "edit_food.h"
#include <QDebug>

NutrientAmountDisplay::NutrientAmountDisplay
  (QWidget* widgetParent, const QSharedPointer<const Nutrient>& nutrient,
   double amount, DisplayModes::DisplayMode mode, bool writeable)
  : QObject(NULL), nutrientAmount(nutrient, amount), displayMode(mode),
    supportsRDI(false), writeable(writeable)
{
  qDebug() << "Constructing NAD from Nutrient";
  initialize(widgetParent);
}

NutrientAmountDisplay::NutrientAmountDisplay
  (QWidget* widgetParent, const NutrientAmount& amount,
   DisplayModes::DisplayMode mode, bool writeable)
  : QObject(NULL), nutrientAmount(amount), displayMode(mode),
    supportsRDI(false), writeable(writeable)
{
  qDebug() << "Constructing NAD from NutrientAmount";
  initialize(widgetParent);
}

NutrientAmountDisplay::NutrientAmountDisplay(const NutrientAmountDisplay& copy)
  : QObject(NULL), nutrientAmount(copy.nutrientAmount), displayMode(copy.displayMode),
    supportsRDI(copy.supportsRDI), writeable(copy.writeable)
{
  qDebug() << "Constructing NAD from copy";

  lblName = copy.lblName;
  txtValue = copy.txtValue;
  lblUnit = copy.lblUnit;

  connect(txtValue, SIGNAL(textEdited(const QString&)), this, SLOT(updateValue(const QString&)));

  setDisplayMode(displayMode, true);
}

NutrientAmountDisplay::~NutrientAmountDisplay()
{
}

void NutrientAmountDisplay::setDisplayMode(DisplayModes::DisplayMode mode)
{
  setDisplayMode(mode, false);
}

void NutrientAmountDisplay::initialize(QWidget* widgetParent)
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

  txtValue->setValidator
    (new QDoubleValidator(0.0, EditFood::MAX_ENTRY, EditFood::MAX_DECIMALS, txtValue));

  txtValue->setReadOnly(!writeable);

  connect(txtValue, SIGNAL(textEdited(const QString&)), this, SLOT(updateValue(const QString&)));

  setDisplayMode(displayMode, true);
}

void NutrientAmountDisplay::setNutrientAmount(const NutrientAmount& newAmount)
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

void NutrientAmountDisplay::setDisplayMode(DisplayModes::DisplayMode mode, bool force)
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

NutrientAmountDisplay& NutrientAmountDisplay::operator=(const NutrientAmountDisplay& rhs)
{
  qDebug() << "Altering NAD through assignment";
  setNutrientAmount(rhs.nutrientAmount);
//  lblName->setParent(rhs.lblName->parentWidget());
//  txtValue->setParent(rhs.txtValue->parentWidget());
//  lblUnit->setParent(rhs.lblUnit->parentWidget());
  return *this;
}

void NutrientAmountDisplay::updateValue(const QString& text)
{
  double value = text.toDouble();

  switch (displayMode)
  {
    case DisplayModes::Weight:
      nutrientAmount.setAmount(value);
      break;
    case DisplayModes::RDI:
      nutrientAmount.setAmountFromPercentRDI(value/100);
      break;
  }
}
