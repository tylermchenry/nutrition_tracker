#include "nutrition_tracker_main.h"
#include "add_food.h"
#include "edit_food.h"
#include "edit_composite_food.h"
#include "edit_template.h"
#include "select_template.h"
#include "instantiate_template.h"
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
  connect(ui.actionAdd_Food_to_Current_Day_from_Template, SIGNAL(triggered()),
           this, SLOT(showAddFoodFromTemplate()));
  connect(ui.actionAdd_new_food, SIGNAL(triggered()),
          this, SLOT(showCreateFood()));
  connect(ui.actionAdd_new_composite_food, SIGNAL(triggered()),
          this, SLOT(showCreateCompositeFood()));
  connect(ui.actionCreate_composite_food_from_template, SIGNAL(triggered()),
          this, SLOT(showCreateCompositeFoodFromTemplate()));
  connect(ui.actionCreate_template, SIGNAL(triggered()),
          this, SLOT(showCreateTemplate()));
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

  if (!infoPrompt.tryAutoConnect()) {
    infoPrompt.exec();
  }

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

void NutritionTrackerMain::showAddFoodFromTemplate()
{
  int mealId;
  FoodAmount instanceAmount = promptForTemplateInstance(true, mealId);

  if (instanceAmount.isDefined() && mealId >= 0) {
    NutritionTracker* nutritionTracker = static_cast<NutritionTracker*>(centralWidget());
    QSharedPointer<CompositeFood> nonceComposite =
      CompositeFood::createNewNonceCompositeFood(instanceAmount);
    nutritionTracker->addFoodToCurrentDay(nonceComposite->getBaseAmount(), mealId);
  }
}

void NutritionTrackerMain::showCreateFood()
{
  QScopedPointer<QDialog>(new EditFood(this))->exec();
}

void NutritionTrackerMain::showCreateCompositeFood()
{
  QScopedPointer<QDialog>(new EditCompositeFood(this))->exec();
}

void NutritionTrackerMain::showCreateCompositeFoodFromTemplate()
{
  FoodAmount instanceAmount = promptForTemplateInstance();

  if (instanceAmount.isDefined()) {
    QScopedPointer<QDialog>(new EditCompositeFood(this, instanceAmount))->exec();
  }
}

void NutritionTrackerMain::showCreateTemplate()
{
  QScopedPointer<QDialog>(new EditTemplate(this))->exec();
}

void NutritionTrackerMain::showMyFoods()
{
  QScopedPointer<QDialog>(new MyFoods(this))->exec();
  tracker->refresh();
}

FoodAmount NutritionTrackerMain::promptForTemplateInstance
  (bool promptForMeal, int& mealId)
{
  QSharedPointer<const Template> templ;
  FoodAmount instanceAmount;

  {
    QScopedPointer<SelectTemplate> selectTemplate(new SelectTemplate(this));
    selectTemplate->exec();
    templ = selectTemplate->getSelectedTemplate();
  }

  if (templ) {
    QSharedPointer<InstantiateTemplate> instantiateTemplate
      (new InstantiateTemplate(templ, promptForMeal, this));
    instantiateTemplate->exec();
    instanceAmount = instantiateTemplate->getInstanceAmount();
    mealId = instantiateTemplate->getSelectedMealId();
  }

  return instanceAmount;
}
