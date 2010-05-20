#ifndef EDIT_FOOD_H
#define EDIT_FOOD_H

#include <QtGui/QDialog>
#include "ui_edit_food.h"
#include "nutrient_amount_display.h"
#include "data/single_food.h"
#include "data/nutrient.h"

class EditFood : public QDialog
{
    Q_OBJECT

public:

    static const double MAX_ENTRY;
    static const int MAX_DECIMALS;

    EditFood(QWidget *parent = 0,
              const QSharedPointer<SingleFood>& food = QSharedPointer<SingleFood>());

    ~EditFood();

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

#endif // EDIT_FOOD_H
