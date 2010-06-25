#include "food_search_control.h"
#include "data/group.h"
#include "data/composite_food.h"
#include "edit_food.h"
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlRecord>
#include <QtSql/QSqlError>
#include <QDebug>

FoodSearchControl::FoodSearchControl(QWidget *parent)
    : QWidget(parent)
{
  ui.setupUi(this);

  connect(ui.btnSearch, SIGNAL(clicked()), this, SLOT(performSearch()));
  connect(ui.btnCreate, SIGNAL(clicked()), this, SLOT(showCreateFood()));

  static const QString COMPOUND_NAME = "(Composite Foods)";
  QSqlDatabase db = QSqlDatabase::database("nutrition_db");

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

    QSqlDatabase db = QSqlDatabase::database("nutrition_db");
    QSqlQuery query(db);

    QString food_sourceRestrictions;

    if (!ui.chkUSDA->isChecked()) {
      food_sourceRestrictions = "Entry_Src != 'USDA'";
    }

    if (!ui.chkImported->isChecked()) {
      if (food_sourceRestrictions != "") food_sourceRestrictions += " AND ";
      food_sourceRestrictions = "Entry_Src != 'Import'";
    }

    if (!ui.chkCustom->isChecked()) {
      // TODO: Implement when users are implemented
      // if (food_sourceRestrictions != "") food_sourceRestrictions += " AND ";
      // food_sourceRestrictions = " (Entry_Src != 'Custom' OR User_Id != :userId)";
    }

    QString composite_sourceRestrictions;

    if (!ui.chkOld->isChecked()) {
      composite_sourceRestrictions = "(ExpiryDate IS NULL OR ExpiryDate > DATE(NOW()))";
    }

    QString categoryRestrictions;

    for (int i = 1; i < ui.lstCategories->count(); ++i) {
      QListWidgetItem* item = ui.lstCategories->item(i);
      if (!ui.lstCategories->isItemSelected(item)) {
        if (categoryRestrictions.size() > 0) {
          categoryRestrictions += " AND ";
        }
        categoryRestrictions += "food_description.FdGrp_Cd != " +
                                 categoryToGroupID[item->text()];
      }
    }

    // For whatever incredibly lame reason, the QMYSQL driver does not seem to
    // support UNION queries, which this should logically be. If I try that,
    // it stops returning results after it hits the first result from the
    // second table in the UNION. Instead, this search is executed as two
    // separate single-table queries, and then the results are "union'ed"
    // client-side by inserting them into a QMap, sorted by description.

    QMap<QString, Result> resultSet;

    QString searchQuery =
        "SELECT Food_Id AS Id, 'Food' AS Type, Long_Desc AS Description,"
        "       NULL AS CreationDate, NULL AS ExpiryDate "
        "  FROM food_description "
        "WHERE " + categoryRestrictions +
        "  " + (categoryRestrictions.size() > 0 ? "AND " : "") + food_sourceRestrictions +
        "  " + (food_sourceRestrictions.size() > 0 ? "AND " : "") + "Long_Desc LIKE "
        "    CONCAT('%', :terms, '%') "
        "ORDER BY Description ASC LIMIT 500";

    runSearchQuery(searchQuery, resultSet);

    if (ui.lstCategories->isItemSelected(ui.lstCategories->item(0))) {

      searchQuery =
          "SELECT Composite_Id AS Id, 'Composite Food' AS Type, Description, "
          "       CreationDate, ExpiryDate"
          "   FROM composite_food "
          "WHERE IsNonce = 0 AND " + composite_sourceRestrictions +
          "  " + (composite_sourceRestrictions.size() > 0 ? "AND " : "") + "Description LIKE "
          "   CONCAT('%', :terms, '%') "
          "ORDER BY Description ASC LIMIT 500";

      runSearchQuery(searchQuery, resultSet);
    }

    for (QMap<QString, Result>::const_iterator i = resultSet.begin(); i != resultSet.end(); ++i) {
      emit newResult(i.value());
    }
  }
}

void FoodSearchControl::showCreateFood()
{
  QScopedPointer<QDialog>(new EditFood(this))->exec();
}

void FoodSearchControl::runSearchQuery(const QString& queryText, QMap<QString, Result>& results) const
{
  QSqlDatabase db = QSqlDatabase::database("nutrition_db");
  QSqlQuery query(db);

  qDebug() << "Query is: " << queryText;

  if (!query.prepare(queryText)) {
    qDebug() << "Prepare failed: " << query.lastError().text();
    return;
  }

  query.bindValue(":terms", ui.txtTerms->text());

  if (!query.exec()) {
    qDebug() << "Exec failed"  << query.lastError().text();
    return;
  }

  qDebug() << "Ran query: " << query.executedQuery();

  int idField = query.record().indexOf("Id");
  int typeField = query.record().indexOf("Type");
  int descField = query.record().indexOf("Description");
  int creationField = query.record().indexOf("CreationDate");
  int expiryField = query.record().indexOf("ExpiryDate");

  while (query.next()) {
    results.insert(query.value(descField).toString(),
                   Result(query.value(idField).toInt(),
                          query.value(typeField).toString(),
                          query.value(descField).toString() +
                          CompositeFood::generateExpirySuffix
                            (query.value(creationField).toDate(),
                             query.value(expiryField).toDate())));
  }
}

uint qHash(const FoodSearchControl::Result& result)
{
  return qHash(result.description);
}
