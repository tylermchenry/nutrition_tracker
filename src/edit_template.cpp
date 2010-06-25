#include "edit_template.h"
#include <QtGui/QMessageBox>
#include <QSettings>
#include <QDebug>

EditTemplate::EditTemplate(QWidget *parent,
                               const QSharedPointer<Template>& food)
    : QDialog(parent), food(food)
{
  ui.setupUi(this);

  populateUserSelector(ui.cboOwner);

  // TODO: Connect owner combobox when it is enabled

  ui.sfcResultsList->setAllowMealSelection(false);

  connect(ui.fscSearch, SIGNAL(beginNewSearch()),
          ui.sfcResultsList, SLOT(clearFoodList()));
  connect(ui.fscSearch, SIGNAL(newResult(const FoodSearchControl::Result&)),
          ui.sfcResultsList, SLOT(addToFoodList(const FoodSearchControl::Result&)));

  connect(ui.sfcResultsList, SIGNAL(amountAdded(const FoodAmount&, int)),
          ui.ftComponents, SLOT(addFoodAmount(const FoodAmount&)));

  connect(ui.btnCancel, SIGNAL(clicked()), this, SLOT(close()));
  connect(ui.btnSave, SIGNAL(clicked()), this, SLOT(saveFood()));
  connect(ui.btnSaveAndAdd, SIGNAL(clicked()), this, SLOT(saveFoodAndClear()));
  connect(ui.btnSaveAndClose, SIGNAL(clicked()), this, SLOT(saveFoodAndClose()));

  ui.ftComponents->setTemporary(true);
  ui.ftComponents->setRootName("Template");
  ui.ftComponents->initialize();

  loadFoodInformation();

  QSettings settings("Nerdland", "Nutrition Tracker");
  settings.beginGroup("edittemplate");
  resize(settings.value("size", size()).toSize());
  if (!settings.value("position").isNull()) {
    move(settings.value("position", pos()).toPoint());
  }
  settings.endGroup();
}

EditTemplate::~EditTemplate()
{
  QSettings settings("Nerdland", "Nutrition Tracker");
  settings.beginGroup("edittemplatefood");
  settings.setValue("size", size());
  settings.setValue("position", pos());
  settings.endGroup();
}

void EditTemplate::clearFood()
{
  food.clear();
  loadFoodInformation();
}

bool EditTemplate::saveFood()
{
  try {

    qDebug() << "Saving template.";

    food->replaceWith(ui.ftComponents->getAllFoods());

    food->setName(ui.txtName->text());

    food->saveToDatabase();

    return true;

  } catch (std::exception& ex) {
    QMessageBox::critical(this, "Failed to save",
                          QString("Unable to save template. Error was: ") + ex.what());
    return false;
  }
}

void EditTemplate::populateUserSelector(QComboBox* cboOwner)
{
  cboOwner->addItem("User");
}

void EditTemplate::loadFoodInformation()
{
  ui.ftComponents->setTemporary(true);

  if (food == NULL) {
    food = Template::createNewTemplate();
  } else {
    ui.ftComponents->setRootCollection(food);
  }

  ui.ftComponents->clear();

  ui.txtName->setText(food->getName());

  // TODO: Set user when users are supported
}

void EditTemplate::saveFoodAndClose()
{
  if (saveFood()) {
    close();
  }
}

void EditTemplate::saveFoodAndClear()
{
  if (saveFood()) {
    clearFood();
  }
}
