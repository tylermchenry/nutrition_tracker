#include "my_foods.h"
#include "edit_food.h"
#include "edit_composite_food.h"
#include "edit_template.h"
#include <QtGui/QMessageBox>

MyFoods::MyFoods(QWidget *parent)
    : QDialog(parent)
{
  ui.setupUi(this);

  ui.cbFilter->addItem("Show all: Single Foods, Composite Foods, and Templates");
  ui.cbFilter->addItem("Show only Single Foods",
                       QVariant::fromValue(FoodCollection::ContainedTypes::SingleFood));
  ui.cbFilter->addItem("Show only Composite Foods",
                       QVariant::fromValue(FoodCollection::ContainedTypes::CompositeFood));
  ui.cbFilter->addItem("Show only Templates",
                       QVariant::fromValue(FoodCollection::ContainedTypes::Template));

  connect(ui.cbFilter, SIGNAL(currentIndexChanged(int)), this, SLOT(loadUserFoods()));

  connect(ui.btnEdit, SIGNAL(clicked()), this, SLOT(edit()));
  connect(ui.btnDuplicate, SIGNAL(clicked()), this, SLOT(duplicate()));
  connect(ui.btnDelete, SIGNAL(clicked()), this, SLOT(del()));
  connect(ui.btnClose, SIGNAL(clicked()), this, SLOT(close()));

  loadUserFoods();
}

MyFoods::~MyFoods()
{
}

void MyFoods::edit()
{
  QListWidgetItem* currentItem = ui.lstFoods->currentItem();

  if (currentItem) {
    QPair<FoodCollection::ContainedTypes::ContainedType, int> food =
      itemToFood[ui.lstFoods->row(currentItem)];

    if (food.first == FoodCollection::ContainedTypes::SingleFood) {
      QSharedPointer<SingleFood> singleFood = SingleFood::getSingleFood(food.second);
      if (singleFood) {
        QScopedPointer<QDialog>(new EditFood(this, singleFood))->exec();
        currentItem->setText(singleFood->getDisplayName());
      } else {
        reportDBError();
      }
    } else if (food.first == FoodCollection::ContainedTypes::CompositeFood) {
      QSharedPointer<CompositeFood> compositeFood = CompositeFood::getCompositeFood(food.second);
      if (compositeFood) {
        QScopedPointer<QDialog>(new EditCompositeFood(this, compositeFood))->exec();
        currentItem->setText(compositeFood->getDisplayName());
      } else {
        reportDBError();
      }
    } else if (food.first == FoodCollection::ContainedTypes::Template) {
      QSharedPointer<Template> compositeFood = Template::getTemplate(food.second);
      if (compositeFood) {
        QScopedPointer<QDialog>(new EditTemplate(this, compositeFood))->exec();
        currentItem->setText(compositeFood->getDisplayName());
      } else {
        reportDBError();
      }
    }
  }
}

void MyFoods::duplicate()
{
  QListWidgetItem* currentItem = ui.lstFoods->currentItem();

  if (currentItem) {
    QPair<FoodCollection::ContainedTypes::ContainedType, int> food =
      itemToFood[ui.lstFoods->row(currentItem)];

    if (food.first == FoodCollection::ContainedTypes::SingleFood) {
      QSharedPointer<SingleFood> baseSingleFood = SingleFood::getSingleFood(food.second);
      if (baseSingleFood) {
        QSharedPointer<SingleFood> singleFood = SingleFood::createNewFood(baseSingleFood);
        singleFood->setName("Copy of " + baseSingleFood->getDisplayName());
        QScopedPointer<QDialog>(new EditFood(this, singleFood))->exec();
      } else {
        reportDBError();
      }
    } else if (food.first == FoodCollection::ContainedTypes::CompositeFood) {
      QSharedPointer<CompositeFood> baseCompositeFood = CompositeFood::getCompositeFood(food.second);
      if (baseCompositeFood) {
        QSharedPointer<CompositeFood> compositeFood = CompositeFood::createNewCompositeFood(baseCompositeFood);
        compositeFood->setName("Copy of " + baseCompositeFood->getDisplayName());
        QScopedPointer<QDialog>(new EditCompositeFood(this, compositeFood))->exec();
      } else {
        reportDBError();
      }
    } else if (food.first == FoodCollection::ContainedTypes::Template) {
      QSharedPointer<Template> baseTemplate = Template::getTemplate(food.second);
      if (baseTemplate) {
        QSharedPointer<Template> compositeFood = Template::createNewTemplate(baseTemplate);
        compositeFood->setName("Copy of " + baseTemplate->getDisplayName());
        QScopedPointer<QDialog>(new EditTemplate(this, compositeFood))->exec();
      } else {
        reportDBError();
      }
    }

    loadUserFoods();
  }
}

void MyFoods::del()
{
  QListWidgetItem* currentItem = ui.lstFoods->currentItem();

  if (currentItem) {
    QPair<FoodCollection::ContainedTypes::ContainedType, int> foodIndex =
      itemToFood[ui.lstFoods->row(currentItem)];

    QSharedPointer<Food> food;

    if (foodIndex.first == FoodCollection::ContainedTypes::SingleFood) {
      food = SingleFood::getSingleFood(foodIndex.second);
    } else if (foodIndex.first == FoodCollection::ContainedTypes::CompositeFood) {
      food = CompositeFood::getCompositeFood(foodIndex.second);
    }

    if (food) {
      if (QMessageBox::warning
            (this, "Confirm Deletion", "Are you sure you want to delete " +
             food->getDisplayName() + "? This cannot be undone.",
             QMessageBox::Yes | QMessageBox::No, QMessageBox::No) == QMessageBox::Yes)
      {
        food->deleteFromDatabase();
        loadUserFoods();
      }
    } else {
      reportDBError();
    }
  }
}

void MyFoods::loadUserFoods()
{
  ui.lstFoods->clear();
  itemToFood.clear();

  QMultiMap<QString, QPair<FoodCollection::ContainedTypes::ContainedType, int> > foods;

  QVariant filter = ui.cbFilter->itemData(ui.cbFilter->currentIndex());

  // TODO: Real user IDs
  if (filter.isNull() || !filter.canConvert<FoodCollection::ContainedTypes::ContainedType>()) {
    foods = FoodCollection::getFoodsForUser(1);
  } else {
    foods = FoodCollection::getFoodsForUser(1, filter.value<FoodCollection::ContainedTypes::ContainedType>());
  }

  for (QMultiMap<QString, QPair<FoodCollection::ContainedTypes::ContainedType, int> >
      ::const_iterator i = foods.begin(); i != foods.end(); ++i)
  {
    ui.lstFoods->addItem(i.key());
    itemToFood[ui.lstFoods->count()-1] = i.value();
  }
}

void MyFoods::reportDBError()
{
  QMessageBox::critical
    (this, "Database Error", "The selected food could not be loaded from the database. "
     "This is an error within the database and there is currently nothing that can be done to correct it. Sorry!");
}
