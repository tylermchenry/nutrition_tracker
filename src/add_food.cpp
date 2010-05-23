#include "add_food.h"
#include "model/food_tree_model.h"
#include <QDebug>

AddFood::AddFood(const QSqlDatabase& db, const QDate& date, QWidget *parent)
    : QDialog(parent), date(date), db(db)
{
	ui.setupUi(this);
	ui.fscSearch->setDatabase(db);

	connect(ui.fscSearch, SIGNAL(beginNewSearch()),
	        ui.sfcResultsList, SLOT(clearFoodList()));
	connect(ui.fscSearch, SIGNAL(newResult(const FoodSearchControl::Result&)),
	        ui.sfcResultsList, SLOT(addToFoodList(const FoodSearchControl::Result&)));

	connect(ui.btnCancel, SIGNAL(clicked()), this, SLOT(close()));
	connect(ui.btnReset, SIGNAL(clicked()), this, SLOT(clearFoods()));
	connect(ui.btnCommit, SIGNAL(clicked()), this, SLOT(commitFoods()));

	ui.trvFoodsToAdd->header()->setResizeMode(QHeaderView::ResizeToContents);

	clearFoods();
}

AddFood::~AddFood()
{
}

void AddFood::expandGrouping(const QModelIndex& index)
{
  ui.trvFoodsToAdd->setExpanded(index, true);
}

void AddFood::clearFoods()
{
  qDebug() << "Clearing foods...";
  FoodTreeModel* foodTreeModel = new FoodTreeModel(ui.trvFoodsToAdd, date, "Proposed Additions");

  connect(ui.sfcResultsList, SIGNAL(amountAdded(const FoodAmount&, int)),
          foodTreeModel, SLOT(addFoodAmount(const FoodAmount&, int)));
  connect(foodTreeModel, SIGNAL(newGroupingCreated(const QModelIndex&)),
          this, SLOT(expandGrouping(const QModelIndex&)));

  ui.trvFoodsToAdd->setModel(foodTreeModel);
}

void AddFood::commitFoods()
{
  FoodTreeModel* foodTreeModel = static_cast<FoodTreeModel*>(ui.trvFoodsToAdd->model());
  emit foodCollectionAdded(foodTreeModel->getAllFoods());
  emit mealsAdded(foodTreeModel->getAllMeals());
  close();
}
