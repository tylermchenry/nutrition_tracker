#ifndef FILL_IN_H
#define FILL_IN_H

#include <QtGui/QDialog>
#include "ui_fill_in.h"
#include "data/nutrient_amount.h"
#include "data/food.h"
#include <QMap>

class FillIn : public QDialog
{
  Q_OBJECT

  public:

    FillIn
      (const QMap<QString, NutrientAmount>& originalNutrients,
       int excludeSingleFoodId = -1, QWidget *parent = 0);

    ~FillIn();

    QMap<QString, NutrientAmount> getFillinNutrients() const;

  private slots:

    void clearFoodList();

    void addToFoodList(const FoodSearchControl::Result& result);

    void updateMetrics();

    void fillIn();

  private:

    Ui::FillInUI ui;

    QMap<QString, NutrientAmount> originalNutrients;
    int excludeSingleFoodId;

    QMap<QString, NutrientAmount> fillinNutrients;
    bool filledIn;

    QMap<int, FoodSearchControl::Result> itemToResult;

    QSharedPointer<Food> getSelectedFood() const;
};

#endif // FILL_IN_H
