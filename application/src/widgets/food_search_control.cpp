#include "food_search_control.h"
#include "libnutrition/data/user.h"
#include "libnutrition/data/group.h"
#include "libnutrition/data/composite_food.h"
#include "libnutrition/backend/back_end.h"
#include "dialogs/edit_food.h"
#include <QDebug>

FoodSearchControl::FoodSearchControl(QWidget *parent)
    : QWidget(parent)
{
  ui.setupUi(this);

  connect(ui.btnSearch, SIGNAL(clicked()), this, SLOT(performSearch()));
  connect(ui.btnCreate, SIGNAL(clicked()), this, SLOT(showCreateFood()));

  static const QString COMPOUND_NAME = "(Composite Foods)";

  ui.lstCategories->clear();
  categoryToGroupID.clear();

  ui.lstCategories->addItem(COMPOUND_NAME);
  categoryToGroupID.insert(COMPOUND_NAME, "");

  QVector<QSharedPointer<const Group> > allGroups = Group::getAllGroups();

  for (QVector<QSharedPointer<const Group> >::const_iterator i = allGroups.begin();
  i != allGroups.end(); ++i)
  {
    ui.lstCategories->addItem((*i)->getName());
    categoryToGroupID.insert((*i)->getName(), (*i)->getId());
  }

  ui.lstCategories->selectAll();
}

FoodSearchControl::~FoodSearchControl()
{

}

void FoodSearchControl::performSearch()
{
  if (ui.txtTerms->text() != "") {

    emit beginNewSearch();

    BackEnd::SearchRequest request;

    request.includeExpired = ui.chkOld->isChecked();
    request.maxResults = 1000;
    request.searchComposites = ui.lstCategories->isItemSelected(ui.lstCategories->item(0));
    request.searchSingleFoods = true;
    request.searchTemplates = false;
    request.searchTerms = ui.txtTerms->text();
    request.sourceImport = ui.chkImported->isChecked();
    request.sourceOthers = ui.chkOthers->isChecked();
    request.sourceSelf = ui.chkCustom->isChecked();
    request.sourceUSDA = ui.chkUSDA->isChecked();

    for (int i = 1; i < ui.lstCategories->count(); ++i) {
      QListWidgetItem* item = ui.lstCategories->item(i);
      if (!ui.lstCategories->isItemSelected(item)) {
        request.excludeGroups.push_back(Group::getGroup(categoryToGroupID[item->text()]));
      }
    }

    QList<BackEnd::SearchResult> results = BackEnd::getBackEnd()->searchFoods(request);

    for (QList<BackEnd::SearchResult>::const_iterator i = results.begin();
         i != results.end(); ++i)
    {
      emit newResult(Result(i->id, BackEnd::FoodTypes::toContainedType(i->foodType),
                            i->displayName));
    }
  }
}

void FoodSearchControl::showCreateFood()
{
  QScopedPointer<QDialog>(new EditFood(this))->exec();
}

uint qHash(const FoodSearchControl::Result& result)
{
  return qHash(result.description);
}
