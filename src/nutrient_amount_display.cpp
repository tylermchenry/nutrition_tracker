/*
 * nutrient_amount_display.cpp
 *
 *  Created on: May 20, 2010
 *      Author: tyler
 */

#include "nutrient_amount_display.h"
#include "edit_food.h"

NutrientAmountDisplay::NutrientAmountDisplay
  (QWidget* widgetParent, const QSharedPointer<const Nutrient>& nutrient,
   double amount, DisplayModes::DisplayMode mode)
  : nutrientAmount(nutrient, amount), displayMode(mode), supportsRDI(false)
{
  initialize(widgetParent);
}

NutrientAmountDisplay::NutrientAmountDisplay
  (QWidget* widgetParent, const NutrientAmount& amount,
   DisplayModes::DisplayMode mode)
  : nutrientAmount(amount), displayMode(mode), supportsRDI(false)
{
  initialize(widgetParent);
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
