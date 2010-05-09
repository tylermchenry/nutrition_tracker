#include "nutrition_tracker_main.h"
#include "nutrition_tracker.h"
#include "add_food.h"
#include "database_information.h"

NutritionTrackerMain::NutritionTrackerMain(QWidget *parent)
    : QMainWindow(parent)
{
  ui.setupUi(this);
  setCentralWidget(new NutritionTracker(this));

  connect(ui.actionAdd_Food_s_to_Current_Day, SIGNAL(triggered()),
          this, SLOT(showAddFood()));
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
}

void NutritionTrackerMain::showAddFood()
{
  (new AddFood(db, this))->exec();
}

