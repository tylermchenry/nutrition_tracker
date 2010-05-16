#ifndef EDIT_FOOD_H
#define EDIT_FOOD_H

#include <QtGui/QDialog>
#include <QtSql/QSqlDatabase>
#include "ui_edit_food.h"
#include "data/nutrient.h"
#include "data/nutrient_amount.h"

class EditFood : public QDialog
{
    Q_OBJECT

public:
    EditFood(const QSqlDatabase& db, QWidget *parent = 0);
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

    QSqlDatabase db;

    QVector<NutrientAmountDisplay> basicNutrients;
    QVector<NutrientAmountDisplay> vitamins;
    QVector<NutrientAmountDisplay> minerals;

    void changeDisplayModes(QVector<NutrientAmountDisplay>& nutrients,
                               NutrientAmountDisplay::DisplayModes::DisplayMode mode);

  private slots:

    void basicNutrientsDimensionChanged(int newIndex);
    void vitaminsDimensionChanged(int newIndex);
    void mineralsDimensionChanged(int newIndex);
};

Q_DECLARE_METATYPE(EditFood::NutrientAmountDisplay::DisplayModes::DisplayMode);

#endif // EDIT_FOOD_H
