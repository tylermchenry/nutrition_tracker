#include "nutrition_tracker_main.h"
#include "nutrition_tracker.h"
#include "add_food.h"
#include "edit_food.h"
#include "database_information.h"
#include <data/single_food.h>
#include <QtGui/QInputDialog>

NutritionTrackerMain::NutritionTrackerMain(QWidget *parent)
    : QMainWindow(parent)
{
  ui.setupUi(this);
  setCentralWidget(new NutritionTracker(this));

  connect(ui.actionAdd_Food_s_to_Current_Day, SIGNAL(triggered()),
          this, SLOT(showAddFood()));
  connect(ui.actionAdd_new_food, SIGNAL(triggered()),
          this, SLOT(showCreateFood()));
  connect(ui.actionEdit_existing_food, SIGNAL(triggered()),
           this, SLOT(showEditFood()));
}

NutritionTrackerMain::~NutritionTrackerMain()
{
}

void NutritionTrackerMain::show()
{
  QMainWindow::show();

  DatabaseInformation infoPrompt(this);
  infoPrompt.exec();

  if (!(db = infoPrompt.getDatabase()).isOpen()) {
    close();
  }

  static_cast<NutritionTracker*>(centralWidget())->initialize();
}

void NutritionTrackerMain::showAddFood()
{
  AddFood* addFood = new AddFood(db, this);
  connect(addFood, SIGNAL(mealsAdded(const QVector<QSharedPointer<const Meal> >&)),
          static_cast<NutritionTracker*>(centralWidget()),
          SLOT(addMealsToCurrentDay(const QVector<QSharedPointer<const Meal> >&)));
  addFood->exec();
}

void NutritionTrackerMain::showCreateFood()
{
  (new EditFood(this))->exec();
}

void NutritionTrackerMain::showEditFood()
{
  QSharedPointer<SingleFood> food =
    SingleFood::getSingleFood(QInputDialog::getInt(this, "Edit Food", "(Temporary) Enter the ID of the food to edit:"));

  if (food != NULL) {
    (new EditFood(this, food))->exec();
  }
}
