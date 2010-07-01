#ifndef VIEW_FOOD_H
#define VIEW_FOOD_H

#include <QtGui/QDialog>
#include "ui_view_food.h"
#include "widgets/nutrient_amount_display.h"
#include "libnutrition/data/single_food.h"
#include "libnutrition/data/nutrient.h"

class ViewFood : public QDialog
{
    Q_OBJECT

public:

    static const double MAX_ENTRY;
    static const int MAX_DECIMALS;

    ViewFood(const FoodAmount& foodAmount, QWidget *parent = 0);

    ~ViewFood();

private:

    Ui::ViewFoodUI ui;

    FoodAmount foodAmount;

    QVector<NutrientAmountDisplay> basicNutrients;
    QVector<NutrientAmountDisplay> vitamins;
    QVector<NutrientAmountDisplay> minerals;

    void populateUnitSelector(QComboBox* cboUnit);

    void populateDimensionSelector(QComboBox* cboDimension);

    void populateNutrientGroup
      (QGroupBox* grpNutrients, QVector<NutrientAmountDisplay>& amountDisplays,
       Nutrient::Categories::Category category);

    void loadNutrientInformation(QVector<NutrientAmountDisplay>& nutrientDisplays,
                                     const QMap<QString, NutrientAmount>& nutrients);

    void loadFoodInformation();

    void changeDisplayModes(QVector<NutrientAmountDisplay>& nutrients,
                               NutrientAmountDisplay::DisplayModes::DisplayMode mode);

  private slots:

    // TODO: Consolodate these with a signal mapper

    void basicNutrientsDimensionChanged(int newIndex);
    void vitaminsDimensionChanged(int newIndex);
    void mineralsDimensionChanged(int newIndex);
};

#endif // VIEW_FOOD_H
