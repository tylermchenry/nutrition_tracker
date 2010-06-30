#include "fill_in.h"
#include "data/single_food.h"
#include "data/composite_food.h"
#include <QSettings>
#include <QSet>
#include <QDebug>
#include <cmath>

FillIn::FillIn(const QMap<QString, NutrientAmount>& originalNutrients,
               int excludeSingleFoodId, QWidget *parent)
  : QDialog(parent), originalNutrients(originalNutrients),
    excludeSingleFoodId(excludeSingleFoodId), filledIn(false)
{
  ui.setupUi(this);

  connect(ui.fscSearch, SIGNAL(beginNewSearch()),
          this, SLOT(clearFoodList()));
  connect(ui.fscSearch, SIGNAL(newResult(const FoodSearchControl::Result&)),
          this, SLOT(addToFoodList(const FoodSearchControl::Result&)));

  connect(ui.lstResults, SIGNAL(currentRowChanged(int)), this, SLOT(updateMetrics()));

  connect(ui.btnCancel, SIGNAL(clicked()), this, SLOT(reject()));
  connect(ui.btnFillIn, SIGNAL(clicked()), this, SLOT(fillIn()));

  QSettings settings("Nerdland", "Nutrition Tracker");
  settings.beginGroup("fillin");
  resize(settings.value("size", size()).toSize());
  if (!settings.value("position").isNull()) {
    move(settings.value("position", pos()).toPoint());
  }
  settings.endGroup();
}

FillIn::~FillIn()
{
  QSettings settings("Nerdland", "Nutrition Tracker");
  settings.beginGroup("fillin");
  settings.setValue("size", size());
  settings.setValue("position", pos());
  settings.endGroup();
}

QMap<QString, NutrientAmount> FillIn::getFillinNutrients() const
{
  if (filledIn) {
    return fillinNutrients;
  } else {
    return QMap<QString, NutrientAmount>();
  }
}

void FillIn::clearFoodList()
{
  ui.lstResults->clear();
  itemToResult.clear();
  updateMetrics();
}

void FillIn::addToFoodList(const FoodSearchControl::Result& result)
{
  if (result.type != "Food" || result.id != excludeSingleFoodId) {
    ui.lstResults->addItem(result.description);
    itemToResult.insert(ui.lstResults->count()-1, result);
  }
}

void FillIn::updateMetrics()
{
  QSharedPointer<Food> selectedFood = getSelectedFood();

  if (!selectedFood) {

    ui.btnFillIn->setEnabled(false);
    ui.txtAmount->setText("");
    ui.txtAverageError->setText("");
    ui.txtLargestError->setText("");
    ui.txtLargestErrorField->setText("");
    ui.txtLargestErrorSelectedVal->setText("");
    ui.txtLargestErrorYourVal->setText("");
    ui.txtNumFields->setText("");
    ui.txtStdDev->setText("");

    fillinNutrients.clear();

  } else {

    ui.btnFillIn->setEnabled(true);

    // TODO: Break this up into multiple methods

    // Step 1. Compute the amount of the selected food needed to give the same
    // number of calories as the original food

    QSharedPointer<const Nutrient> calories = Nutrient::getNutrientByName(Nutrient::CALORIES_NAME);

    FoodAmount selectedBaseAmount = selectedFood->getBaseAmount();

    double caloriesInOriginal = originalNutrients[calories->getId()].getAmount();
    double caloriesInSelectedBaseAmount =
      selectedBaseAmount.getScaledNutrients()[calories->getId()].getAmount();

    qDebug() << "Calories in original: " << caloriesInOriginal;
    qDebug() << "Calories in selected base amount: " << selectedBaseAmount.getScaledNutrients()[calories->getId()].getAmount();

    FoodAmount selectedAmount =
      selectedBaseAmount * (caloriesInOriginal / caloriesInSelectedBaseAmount);

    ui.txtAmount->setText(QString::number(selectedAmount.getAmount(), 'f', 2) + " " +
                           selectedAmount.getUnit()->getAbbreviation());


    // Step 2. Determine which nutrients are in common, and which can be filled in

    QMap<QString, NutrientAmount> selectedNutrients = selectedAmount.getScaledNutrients();

    QSet<QString> originalNutrientIds = originalNutrients.keys().toSet();
    QSet<QString> selectedNutrientIds = selectedNutrients.keys().toSet();

    QSet<QString> commonNutrientIds = originalNutrientIds;
    commonNutrientIds.intersect(selectedNutrientIds);

    QSet<QString> fillinNutrientIds = selectedNutrientIds;
    fillinNutrientIds.subtract(originalNutrientIds);

    qDebug() << "Original nutrients: " << originalNutrientIds.size();
    qDebug() << "Selected nutrients: " << selectedNutrientIds.size();
    qDebug() << "Common nutrients: " << commonNutrientIds.size();
    qDebug() << "Fill-in nutrients: " << fillinNutrientIds.size();

    ui.txtNumFields->setText(QString::number(fillinNutrientIds.size()));

    // Step 3. Compute the percent error for each common nutrient, and compute
    // the average, also making note of the largest.

    QMap<QString, double> pctError;
    double sumOfErrors = 0;

    QString largestErrorId;
    double largestError = 0;

    for (QSet<QString>::const_iterator i = commonNutrientIds.begin(); i != commonNutrientIds.end(); ++i) {
      double originalValue = originalNutrients[*i].getAmount();

      if (originalValue > 0) {

        double selectedValue = selectedNutrients[*i].getAmount();
        double error = std::fabs(originalValue - selectedValue) / originalValue;
        pctError[*i] = error;
        sumOfErrors += error;

        if (error > largestError) {
          largestErrorId = *i;
          largestError = error;
        }

        qDebug() << "Error for common nutrient " << *i << " ("
                  << originalNutrients[*i].getNutrient()->getName() << ") is "
                  << error;
      }
    }

    double avgError = sumOfErrors / pctError.size();

    ui.txtAverageError->setText(QString::number(avgError * 100, 'f', 2) + " %");

    ui.txtLargestError->setText(QString::number(largestError * 100, 'f', 2) + " %");
    ui.txtLargestErrorField->setText(originalNutrients[largestErrorId].getNutrient()->getName());

    ui.txtLargestErrorYourVal->setText
      (QString::number(originalNutrients[largestErrorId].getAmount(), 'f', 2) + " " +
       originalNutrients[largestErrorId].getUnit()->getAbbreviation());

    ui.txtLargestErrorSelectedVal->setText
      (QString::number(selectedNutrients[largestErrorId].getAmount(), 'f', 2) + " " +
       selectedNutrients[largestErrorId].getUnit()->getAbbreviation());


    // Step 4. Compute the standard deviation of the error percentages

    double sumOfDistancesSquared = 0;

    for (QMap<QString, double>::const_iterator i = pctError.begin(); i != pctError.end(); ++i) {
      double distance = i.value() - avgError;
      sumOfDistancesSquared += (distance * distance);
    }

    double stdDev = std::sqrt(sumOfDistancesSquared / pctError.size());

    ui.txtStdDev->setText(QString::number(stdDev, 'f', 2));

    // Step 5. Save a copy of all of the fill-in nutrient amounts

    fillinNutrients.clear();
    for (QSet<QString>::const_iterator i = fillinNutrientIds.begin(); i != fillinNutrientIds.end(); ++i) {
      fillinNutrients[*i] = selectedNutrients[*i];
    }
  }
}

void FillIn::fillIn()
{
  filledIn = true;
  accept();
}

QSharedPointer<Food> FillIn::getSelectedFood() const
{
  QSharedPointer<Food> selectedFood;

  if (ui.lstResults->currentItem() != NULL) {
    const FoodSearchControl::Result& result = itemToResult[ui.lstResults->currentIndex().row()];

    if (result.type == "Food") {
      selectedFood = SingleFood::getSingleFood(result.id);
    } else {
      selectedFood = CompositeFood::getCompositeFood(result.id);
    }
  }

  return selectedFood;
}

