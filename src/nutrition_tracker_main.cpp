#include "nutrition_tracker_main.h"
#include "add_food.h"
#include "edit_food.h"
#include "edit_composite_food.h"
#include "my_foods.h"
#include "database_information.h"
#include <data/single_food.h>
#include <QtGui/QInputDialog>

NutritionTrackerMain::NutritionTrackerMain(QWidget *parent)
    : QMainWindow(parent)
{
  ui.setupUi(this);

  tracker = new NutritionTracker(this);

  setCentralWidget(tracker);

  connect(ui.actionAdd_Food_s_to_Current_Day, SIGNAL(triggered()),
          this, SLOT(showAddFood()));
  connect(ui.actionAdd_new_food, SIGNAL(triggered()),
          this, SLOT(showCreateFood()));
  connect(ui.actionAdd_new_composite_food, SIGNAL(triggered()),
           this, SLOT(showCreateCompositeFood()));
  connect(ui.actionManage_my_foods, SIGNAL(triggered()),
          this, SLOT(showMyFoods()));
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
  NutritionTracker* nutritionTracker = static_cast<NutritionTracker*>(centralWidget());
  QScopedPointer<AddFood> addFood(new AddFood(nutritionTracker->getSelectedDate(), this));
  connect(addFood.data(), SIGNAL(mealsAdded(const QVector<QSharedPointer<const Meal> >&)),
          nutritionTracker, SLOT(addMealsToCurrentDay(const QVector<QSharedPointer<const Meal> >&)));
  addFood->exec();
}

void NutritionTrackerMain::showCreateFood()
{
  QScopedPointer<QDialog>(new EditFood(this))->exec();
}

void NutritionTrackerMain::showCreateCompositeFood()
{
  QScopedPointer<QDialog>(new EditCompositeFood(this))->exec();
}

void NutritionTrackerMain::showMyFoods()
{
  QScopedPointer<QDialog>(new MyFoods(this))->exec();
  tracker->refresh();
}
