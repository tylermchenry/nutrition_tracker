#include "nutrition_tracker_main.h"
#include "add_food.h"
#include "edit_food.h"
#include "edit_composite_food.h"
#include "edit_template.h"
#include "select_template.h"
#include "instantiate_template.h"
#include "my_foods.h"
#include "about.h"
#include "database_information.h"
#include "user_login.h"
#include <data/single_food.h>
#include <QtGui/QInputDialog>
#include <QtGui/QLabel>
#include <QtGui/QStatusBar>
#include <QSettings>

NutritionTrackerMain::NutritionTrackerMain(QWidget *parent)
    : QMainWindow(parent)
{
  ui.setupUi(this);

  tracker = new NutritionTracker(this);

  setCentralWidget(tracker);

  connect(tracker, SIGNAL(addFood()), this, SLOT(showAddFood()));
  connect(tracker, SIGNAL(addFoodFromTemplate()), this, SLOT(showAddFoodFromTemplate()));
  connect(tracker, SIGNAL(createFood()), this, SLOT(showCreateFood()));
  connect(tracker, SIGNAL(createCompositeFood()), this, SLOT(showCreateCompositeFood()));

  setStatusBar(new QStatusBar());

  connect(ui.actionExit, SIGNAL(triggered()), this, SLOT(close()));
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
  connect(ui.actionAbout_Nutrition_Tracker, SIGNAL(triggered()),
          this, SLOT(showAbout()));
}

NutritionTrackerMain::~NutritionTrackerMain()
{
  QSettings settings("Nerdland", "Nutrition Tracker");
  settings.beginGroup("mainwindow");
  settings.setValue("size", size());
  settings.setValue("position", pos());
  settings.setValue("fullscreen", isFullScreen());
  settings.endGroup();
}

void NutritionTrackerMain::show()
{
  QSettings settings("Nerdland", "Nutrition Tracker");

  settings.beginGroup("mainwindow");

  resize(settings.value("size", size()).toSize());
  if (!settings.value("position").isNull()) {
    move(settings.value("position", pos()).toPoint());
  }

  if (settings.value("fullscreen", false).toBool()) {
    showFullScreen();
  } else {
    QMainWindow::show();
  }

  settings.endGroup();

  DatabaseInformation infoPrompt(this);

  if (!infoPrompt.tryAutoConnect()) {
    infoPrompt.exec();
  }

  if (!(db = infoPrompt.getDatabase()).isOpen()) {
    close();
  }

  UserLogin loginPrompt(this);

  if (!loginPrompt.tryAutoLogIn()) {
    loginPrompt.exec();
  }

  if (!loginPrompt.loggedIn()) {
    close();
  }

  QString statusMessage;

  statusMessage = "Using database " + db.databaseName() + " on " +
    db.userName() + "@" + db.hostName() + ". Logged in as " +
    User::getLoggedInUser()->getDisplayName() + ".";


  statusBar()->addPermanentWidget(new QLabel(statusMessage, statusBar()), 100);

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

void NutritionTrackerMain::showAbout()
{
  QScopedPointer<QDialog>(new About(this))->exec();
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
