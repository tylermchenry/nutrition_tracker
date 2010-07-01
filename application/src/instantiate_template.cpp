#include "instantiate_template.h"
#include "libnutrition/data/meal.h"
#include <QSettings>

InstantiateTemplate::InstantiateTemplate
  (const QSharedPointer<const Template>& templ, bool promptForMeal, QWidget *parent)
    : QDialog(parent), tcComponents(new TemplateComponents(templ, this)),
      promptForMeal(promptForMeal), created(false)
{
  ui.setupUi(this);

  ui.txtName->setText(templ->getName());

  ui.txtAmount->setText("1");

  QVector<QSharedPointer<const Unit> > allUnits = Unit::getAllUnits();

  for (QVector<QSharedPointer<const Unit> >::const_iterator i = allUnits.begin();
  i != allUnits.end(); ++i)
  {
    ui.cbAmountUnit->addItem((*i)->getNameAndAbbreviation(), (*i)->getAbbreviation());
  }

  ui.cbAmountUnit->setCurrentIndex
  (ui.cbAmountUnit->findData
   (Unit::getPreferredUnit(Unit::Dimensions::Serving)->getAbbreviation()));

  ui.lblMeal->setVisible(promptForMeal);
  ui.cbMeal->setVisible(promptForMeal);

  if (promptForMeal) {
    QMap<int, QString> mealNames = Meal::getAllMealNames();

    for (QMap<int, QString>::const_iterator i = mealNames.begin(); i != mealNames.end(); ++i)
    {
      ui.cbMeal->addItem(i.value(), i.key());
    }
  }

  ui.saComponents->setWidget(tcComponents);

  connect(ui.btnCancel, SIGNAL(clicked()), this, SLOT(close()));
  connect(ui.btnCreate, SIGNAL(clicked()), this, SLOT(create()));

  QSettings settings("Nerdland", "Nutrition Tracker");
  settings.beginGroup("instantiatetemplate");
  resize(settings.value("size", size()).toSize());
  if (!settings.value("position").isNull()) {
    move(settings.value("position", pos()).toPoint());
  }
  settings.endGroup();
}

InstantiateTemplate::~InstantiateTemplate()
{
  QSettings settings("Nerdland", "Nutrition Tracker");
  settings.beginGroup("instantiatetemplate");
  settings.setValue("size", size());
  settings.setValue("position", pos());
  settings.endGroup();
}

FoodAmount InstantiateTemplate::getInstanceAmount() const
{
  if (created) {
    return FoodAmount(tcComponents->getCollection(),
                      ui.txtAmount->text().toDouble(),
                      Unit::getUnit(ui.cbAmountUnit->itemData
                                     (ui.cbAmountUnit->currentIndex()).toString()));
  } else {
    return FoodAmount();
  }
}

int InstantiateTemplate::getSelectedMealId() const
{
  if (created && promptForMeal) {
    return ui.cbMeal->itemData(ui.cbMeal->currentIndex()).toInt();
  } else {
    return -1;
  }
}

void InstantiateTemplate::create()
{
  created = true;
  close();
}
