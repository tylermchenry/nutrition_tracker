#include "food_tree.h"
#include "dialogs/view_food.h"
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

void FoodTree::setRootCollection
  (const QSharedPointer<FoodCollection>& collection)
{
  rootCollection = collection;
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
  if (rootCollection) {
    model.reset(new FoodTreeModel(ui.trvFoods, date, rootCollection, temporary));
  } else {
    model.reset(new FoodTreeModel(ui.trvFoods, date, rootName, temporary));
  }

  connect(model.data(), SIGNAL(newGroupingCreated(const QModelIndex&)),
          this, SLOT(expandGrouping(const QModelIndex&)));

  connect(model.data(), SIGNAL(dataChanged(const QModelIndex&, const QModelIndex&)),
          this, SIGNAL(contentsModified()));

  connect(model.data(), SIGNAL(rowsInserted(const QModelIndex&, int, int)),
           this, SIGNAL(contentsModified()));

  connect(model.data(), SIGNAL(rowsRemoved(const QModelIndex&, int, int)),
           this, SIGNAL(contentsModified()));

  ui.trvFoods->setModel(model.data());
  ui.trvFoods->setExpanded(model->getRootCollectionIndex(), true);
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
  QScopedPointer<FoodContextMenu> contextMenu;

  contextMenu.reset(model->getContextMenu(ui.trvFoods->indexAt(point)));

  if (contextMenu) {
    qDebug() << "Showing context menu";
    connect(contextMenu.data(), SIGNAL(remove(const QModelIndex&, FoodComponent*)),
            this, SLOT(removeComponent(const QModelIndex&, FoodComponent*)));
    connect(contextMenu.data(), SIGNAL(changeAmount(const QModelIndex&, FoodComponent*)),
             this, SLOT(changeAmount(const QModelIndex&, FoodComponent*)));
    connect(contextMenu.data(), SIGNAL(changeUnit(const QModelIndex&, FoodComponent*, const QSharedPointer<const Unit>&)),
             this, SLOT(changeUnit(const QModelIndex&, FoodComponent*, const QSharedPointer<const Unit>&)));
    connect(contextMenu.data(), SIGNAL(viewNutritionInformation(const QModelIndex&, const FoodAmount&)),
             this, SLOT(displayNutritionInfo(const QModelIndex&, const FoodAmount&)));
    connect(contextMenu.data(), SIGNAL(copyMealToDay(const QModelIndex&, const QSharedPointer<const Meal>&, const QDate&)),
             this, SLOT(copyMealToDay(const QModelIndex&, const QSharedPointer<const Meal>&, const QDate&)));
    contextMenu->exec(ui.trvFoods->viewport()->mapToGlobal(point));
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
    FoodAmount amount = component->getFoodAmount();

    double newAmount =
        QInputDialog::getDouble(this, "Change Amount",
                                "New amount (in " + amount.getUnit()->getName() + "):",
                                 amount.getAmount(), 0);
   model->changeAmount(index, FoodAmount(amount.getFood(), newAmount, amount.getUnit()));
  }
}

void FoodTree::changeUnit(const QModelIndex& index, FoodComponent* component,
                             const QSharedPointer<const Unit>& unit)
{
  if (component && unit) {
    FoodAmount amount = component->getFoodAmount();

    double newAmount = amount.getAmount(unit);

    model->changeAmount(index, FoodAmount(amount.getFood(), newAmount, unit));
  }
}

void FoodTree::copyMealToDay(const QModelIndex&, const QSharedPointer<const Meal>& meal,
                                 const QDate& date)
{
  if (meal && !meal->isTemporary()) {
    QSharedPointer<Meal> targetMeal = Meal::getOrCreateMeal(meal->getOwnerId(), date, meal->getMealId());
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
