#ifndef EDIT_FOOD_H
#define EDIT_FOOD_H

#include <QtGui/QDialog>
#include "ui_edit_food.h"
#include "widgets/nutrient_amount_display.h"
#include "libnutrition/data/single_food.h"
#include "libnutrition/data/nutrient.h"

class EditFood : public QDialog
{
    Q_OBJECT

public:

    static const double MAX_ENTRY;
    static const int MAX_DECIMALS;

    EditFood(QWidget *parent = 0,
              const QSharedPointer<SingleFood>& food = QSharedPointer<SingleFood>());

    ~EditFood();

public slots:

    void clearFood();

    bool saveFood();

private slots:

    void promptForFillIn();

private:

    Ui::EditFoodUI ui;

    QSharedPointer<SingleFood> food;

    QVector<NutrientAmountDisplay> basicNutrients;
    QVector<NutrientAmountDisplay> vitamins;
    QVector<NutrientAmountDisplay> minerals;

    // A temporary way to hold filled-in "extra" nutrients until we have a
    // way to display them
    QMap<QString, NutrientAmount> leftoverFillins;

    void populateSourceSelector(QComboBox* cboSource);

    void populateUserSelector(QComboBox* cboOwner);

    void populateGroupSelector(QComboBox* cboGroup);

    void populateUnitSelector(QComboBox* cboUnit, Unit::Dimensions::Dimension dimension);

    void populateDimensionSelector(QComboBox* cboDimension);

    QWidget* populateNutrientGroup
      (QGroupBox* grpNutrients, QVector<NutrientAmountDisplay>& amountDisplays,
       Nutrient::Categories::Category category, QWidget* previousTabWidget);

    void loadAmountInformation(QLineEdit* txtAmount, QComboBox* cboUnit,
                                   Unit::Dimensions::Dimension dimension);

    void loadNutrientInformation(QVector<NutrientAmountDisplay>& nutrientDisplays,
                                     const QMap<QString, NutrientAmount>& nutrients);

    void loadFoodInformation();

    void saveNutrientInformation(const QVector<NutrientAmountDisplay>& nutrientDisplays);

    QMap<QString, NutrientAmount>& addNutrientInformation
      (const QVector<NutrientAmountDisplay>& nutrientDisplays,
       QMap<QString, NutrientAmount>& nutrientMap);

    void fillInFrom(QVector<NutrientAmountDisplay>& nutrientDisplays,
                      QMap<QString, NutrientAmount>& nutrientMap);

    void changeDisplayModes(QVector<NutrientAmountDisplay>& nutrients,
                               NutrientAmountDisplay::DisplayModes::DisplayMode mode);

  private slots:

    // TODO: Consolodate these with a signal mapper

    void basicNutrientsDimensionChanged(int newIndex);
    void vitaminsDimensionChanged(int newIndex);
    void mineralsDimensionChanged(int newIndex);

    void saveFoodAndClose();
    void saveFoodAndClear();
};

#endif // EDIT_FOOD_H
