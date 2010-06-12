#include "food_tree.h"
#include "view_food.h"
#include <QDebug>
#include <QtGui/QInputDialog>

FoodTree::FoodTree(QWidget *parent)
  : QWidget(parent), temporary(true), rootName("Foods"),
    date(QDate::currentDate()), model(NULL)
{
  ui.setupUi(this);
}

FoodTree::FoodTree(const QString& rootName, const QDate& date, QWidget *parent)
  : QWidget(parent), temporary(true), rootName(rootName),
    date(date), model(NULL)
{
  ui.setupUi(this);
}

FoodTree::~FoodTree()
{
}

void FoodTree::setTemporary(bool temporary)
{
  this->temporary = temporary;
}

void FoodTree::setRootName(const QString& newRootName)
{
  rootName = newRootName;
}

void FoodTree::setDate(const QDate& newDate)
{
  date = newDate;
}

QSharedPointer<const FoodCollection> FoodTree::getAllFoods() const
{
  return model->getAllFoods();
}

QVector<QSharedPointer<const Meal> > FoodTree::getAllMeals() const
{
  return model->getAllMeals();
}

void FoodTree::clear()
{
  model = new FoodTreeModel(ui.trvFoods, date, rootName, temporary);

  connect(model, SIGNAL(newGroupingCreated(const QModelIndex&)),
          this, SLOT(expandGrouping(const QModelIndex&)));

  ui.trvFoods->setModel(model);
}

void FoodTree::addFoodAmount(const FoodAmount& foodAmount)
{
  model->addFoodAmount(foodAmount);
}

void FoodTree::addFoodAmount(const FoodAmount& foodAmount, int mealId)
{
  model->addFoodAmount(foodAmount, mealId);
}

void FoodTree::addMeal(const QSharedPointer<const Meal>& meal)
{
  model->addMeal(meal);
}

void FoodTree::addMeals(const QVector<QSharedPointer<const Meal> >& meals)
{
  for (QVector<QSharedPointer<const Meal> >::const_iterator i = meals.begin(); i != meals.end(); ++i) {
    addMeal(*i);
  }
}

void FoodTree::addMeals(const QVector<QSharedPointer<Meal> >& meals)
{
  for (QVector<QSharedPointer<Meal> >::const_iterator i = meals.begin(); i != meals.end(); ++i) {
    addMeal(*i);
  }
}

void FoodTree::expandGrouping(const QModelIndex& index)
{
  ui.trvFoods->setExpanded(index, true);
}

void FoodTree::showContextMenu(const QPoint& point)
{
  FoodContextMenu* contextMenu = model->getContextMenu(ui.trvFoods->indexAt(point));

  if (contextMenu) {
    qDebug() << "Showing context menu";
    connect(contextMenu, SIGNAL(remove(const QModelIndex&, FoodComponent*)),
            this, SLOT(removeComponent(const QModelIndex&, FoodComponent*)));
    connect(contextMenu, SIGNAL(changeAmount(const QModelIndex&, FoodComponent*)),
             this, SLOT(changeAmount(const QModelIndex&, FoodComponent*)));
    connect(contextMenu, SIGNAL(viewNutritionInformation(const QModelIndex&, const FoodAmount&)),
             this, SLOT(displayNutritionInfo(const QModelIndex&, const FoodAmount&)));
    connect(contextMenu, SIGNAL(copyMealToDay(const QModelIndex&, const QSharedPointer<const Meal>&, const QDate&)),
             this, SLOT(copyMealToDay(const QModelIndex&, const QSharedPointer<const Meal>&, const QDate&)));
    contextMenu->popup(ui.trvFoods->viewport()->mapToGlobal(point));
  }
}

void FoodTree::removeComponent(const QModelIndex&, FoodComponent* component)
{
  if (component) {
    model->removeComponent(*component);
  }
}

void FoodTree::displayNutritionInfo(const QModelIndex&, const FoodAmount& amount)
{
  (new ViewFood(amount, this))->exec();
}

void FoodTree::changeAmount(const QModelIndex& index, FoodComponent* component)
{
  if (component) {
    const FoodAmount& amount = component->getFoodAmount();

    double newAmount =
        QInputDialog::getDouble(this, "Change Amount",
                                "New amount (in " + amount.getUnit()->getName() + "):",
                                 amount.getAmount(), 0);
   model->changeAmount(index, FoodAmount(amount.getFood(), newAmount, amount.getUnit()));
  }
}

void FoodTree::copyMealToDay(const QModelIndex&, const QSharedPointer<const Meal>& meal,
                                 const QDate& date)
{
  if (meal && !meal->isTemporary()) {
    QSharedPointer<Meal> targetMeal = Meal::getOrCreateMeal(meal->getUserId(), date, meal->getMealId());
    targetMeal->merge(meal);
    targetMeal->saveToDatabase();
  }
}

void FoodTree::initialize()
{
  ui.trvFoods->header()->setResizeMode(QHeaderView::ResizeToContents);

  ui.trvFoods->setContextMenuPolicy(Qt::CustomContextMenu);

  connect(ui.trvFoods, SIGNAL(customContextMenuRequested(const QPoint&)),
          this, SLOT(showContextMenu(const QPoint&)));

  clear();
}
