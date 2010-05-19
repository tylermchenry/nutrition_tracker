#ifndef EDIT_FOOD_H
#define EDIT_FOOD_H

#include <QtGui/QDialog>
#include "ui_edit_food.h"
#include "data/single_food.h"
#include "data/nutrient.h"
#include "data/nutrient_amount.h"

class EditFood : public QDialog
{
    Q_OBJECT

public:
    EditFood(QWidget *parent = 0,
              const QSharedPointer<SingleFood>& food = QSharedPointer<SingleFood>());
    ~EditFood();

    class NutrientAmountDisplay
    {
      public:

        struct DisplayModes {
          enum DisplayMode { Weight, RDI };
        };

        explicit NutrientAmountDisplay(QWidget* widgetParent = NULL,
                                           const NutrientAmount& amount = NutrientAmount(),
                                           DisplayModes::DisplayMode mode = DisplayModes::Weight);

        NutrientAmountDisplay(QWidget* widgetParent,
                                const QSharedPointer<const Nutrient>& nutrient,
                                double amount,
                                DisplayModes::DisplayMode mode = DisplayModes::Weight);

        inline QWidget* getNameWidget() const { return lblName; }

        inline QWidget* getValueWidget() const {return txtValue; }

        inline QWidget* getUnitWidget() const { return lblUnit; }

        inline const NutrientAmount& getNutrientAmount() const { return nutrientAmount; }

        void setNutrientAmount(const NutrientAmount& newAmount);

        void setDisplayMode(DisplayModes::DisplayMode mode);

      private:

        NutrientAmount nutrientAmount;
        DisplayModes::DisplayMode displayMode;
        bool supportsRDI;

        QLabel* lblName;
        QLineEdit* txtValue;
        QLabel* lblUnit;

        void initialize(QWidget* widgetParent);

        void setDisplayMode(DisplayModes::DisplayMode mode, bool force);
    };

private:
    Ui::EditFoodUI ui;

    QSharedPointer<SingleFood> food;

    QVector<NutrientAmountDisplay> basicNutrients;
    QVector<NutrientAmountDisplay> vitamins;
    QVector<NutrientAmountDisplay> minerals;

    void populateSourceSelector(QComboBox* cboSource);

    void populateUserSelector(QComboBox* cboOwner);

    void populateGroupSelector(QComboBox* cboGroup);

    void populateUnitSelector(QComboBox* cboUnit, Unit::Dimensions::Dimension dimension);

    void populateDimensionSelector(QComboBox* cboDimension);

    void populateNutrientGroup
      (QGroupBox* grpNutrients, QVector<NutrientAmountDisplay>& amountDisplays,
       Nutrient::Categories::Category category);

    void loadAmountInformation(QLineEdit* txtAmount, QComboBox* cboUnit,
                                   Unit::Dimensions::Dimension dimension);

    void loadNutrientInformation(QVector<NutrientAmountDisplay>& nutrientDisplays,
                                     const QMap<QString, NutrientAmount>& nutrients);

    void loadFoodInformation();

    void changeDisplayModes(QVector<NutrientAmountDisplay>& nutrients,
                               NutrientAmountDisplay::DisplayModes::DisplayMode mode);

  private slots:

    void basicNutrientsDimensionChanged(int newIndex);
    void vitaminsDimensionChanged(int newIndex);
    void mineralsDimensionChanged(int newIndex);
};

Q_DECLARE_METATYPE(EditFood::NutrientAmountDisplay::DisplayModes::DisplayMode);

#endif // EDIT_FOOD_H
