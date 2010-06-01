/*
 * nutrient_amount_display.h
 *
 *  Created on: May 20, 2010
 *      Author: tyler
 */

#ifndef NUTRIENT_AMOUNT_DISPLAY_H_
#define NUTRIENT_AMOUNT_DISPLAY_H_

#include "data/nutrient.h"
#include "data/nutrient_amount.h"
#include <QObject>
#include <QVariant>
#include <QtGui/QWidget>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>

class NutrientAmountDisplay : public QObject
{
    Q_OBJECT

  public:

    struct DisplayModes {
      enum DisplayMode { Weight, RDI };
    };

    explicit NutrientAmountDisplay(QWidget* widgetParent = NULL,
                                   const NutrientAmount& amount = NutrientAmount(),
                                   DisplayModes::DisplayMode mode = DisplayModes::Weight,
                                   bool enabled = true);

    NutrientAmountDisplay(QWidget* widgetParent,
                          const QSharedPointer<const Nutrient>& nutrient,
                          double amount,
                          DisplayModes::DisplayMode mode = DisplayModes::Weight,
                          bool enabled = true);

    NutrientAmountDisplay(const NutrientAmountDisplay& copy);

    virtual ~NutrientAmountDisplay();

    inline QWidget* getNameWidget() const { return lblName; }

    inline QWidget* getValueWidget() const {return txtValue; }

    inline QWidget* getUnitWidget() const { return lblUnit; }

    inline const NutrientAmount& getNutrientAmount() const { return nutrientAmount; }

    void setNutrientAmount(const NutrientAmount& newAmount);

    void setDisplayMode(DisplayModes::DisplayMode mode);

    NutrientAmountDisplay& operator=(const NutrientAmountDisplay& rhs);

  private:

    NutrientAmount nutrientAmount;
    DisplayModes::DisplayMode displayMode;
    bool supportsRDI;
    bool enabled;

    QLabel* lblName;
    QLineEdit* txtValue;
    QLabel* lblUnit;

    void initialize(QWidget* widgetParent);

    void setDisplayMode(DisplayModes::DisplayMode mode, bool force);

  private slots:

    void updateValue(const QString& text);
};

Q_DECLARE_METATYPE(NutrientAmountDisplay::DisplayModes::DisplayMode);

#endif /* NUTRIENT_AMOUNT_DISPLAY_H_ */
