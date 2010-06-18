#include "my_foods.h"
#include "edit_food.h"
#include "edit_composite_food.h"

MyFoods::MyFoods(QWidget *parent)
    : QDialog(parent)
{
  ui.setupUi(this);

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
      QScopedPointer<QDialog>(new EditFood(this, singleFood))->exec();
      currentItem->setText(singleFood->getName());
    } else if (food.first == FoodCollection::ContainedTypes::CompositeFood) {
      QSharedPointer<CompositeFood> compositeFood = CompositeFood::getCompositeFood(food.second);
      QScopedPointer<QDialog>(new EditCompositeFood(this, compositeFood))->exec();
      currentItem->setText(compositeFood->getName());
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
      QSharedPointer<SingleFood> singleFood = SingleFood::createNewFood(baseSingleFood);
      singleFood->setName("Copy of " + baseSingleFood->getName());
      QScopedPointer<QDialog>(new EditFood(this, singleFood))->exec();
    } else if (food.first == FoodCollection::ContainedTypes::CompositeFood) {
      QSharedPointer<CompositeFood> baseCompositeFood = CompositeFood::getCompositeFood(food.second);
      QSharedPointer<CompositeFood> compositeFood = CompositeFood::createNewCompositeFood(baseCompositeFood);
      compositeFood->setName("Copy of " + baseCompositeFood->getName());
      QScopedPointer<QDialog>(new EditCompositeFood(this, compositeFood))->exec();
    }

    loadUserFoods();
  }
}

void MyFoods::del()
{
  // TODO: Implement when deletion functionality exists at data layer
}

void MyFoods::loadUserFoods()
{
  ui.lstFoods->clear();
  itemToFood.clear();

  QMultiMap<QString, QPair<FoodCollection::ContainedTypes::ContainedType, int> > foods =
    FoodCollection::getFoodsForUser(1); // TODO: Real user ID

  for (QMultiMap<QString, QPair<FoodCollection::ContainedTypes::ContainedType, int> >
      ::const_iterator i = foods.begin(); i != foods.end(); ++i)
  {
    ui.lstFoods->addItem(i.key());
    itemToFood[ui.lstFoods->count()-1] = i.value();
  }
}
