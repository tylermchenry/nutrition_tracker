#include "food_search_control.h"
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlRecord>
#include <QtSql/QSqlError>
#include <QDebug>

FoodSearchControl::FoodSearchControl(QWidget *parent)
    : QWidget(parent)
{
	ui.setupUi(this);

	connect(ui.btnSearch, SIGNAL(clicked()), this, SLOT(performSearch()));
}

FoodSearchControl::~FoodSearchControl()
{

}

void FoodSearchControl::performSearch()
{
  if (ui.txtTerms->text() != "") {

    emit beginNewSearch();

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

    if (!ui.chkCustom->isChecked()) {
      // TODO: Implement when users are implemented
      // if (food_sourceRestrictions != "") food_sourceRestrictions += " AND ";
      // food_sourceRestrictions = " (Entry_Src != 'Custom' OR User_Id == :userId)";
    }

    QString composite_sourceRestrictions;

    if (!ui.chkOld->isChecked()) {
      // TODO: Implement when old markin gis implemented
      // composite_sourceRestrictions = "Old == 0";
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
    // client-side by inserting them into a std::set, sorted by description.
    // Note: a QSet was not used because Result is a private inner class and
    // I did not want to make it public for the purpose of implementing qHash.

    std::set<Result> resultSet;

    QString searchQuery =
        "SELECT Food_Id AS Id, 'Food' AS Type, Long_Desc AS Description"
        "  FROM food_description "
        "WHERE " + categoryRestrictions +
        "  " + (categoryRestrictions.size() > 0 ? "AND " : "") + food_sourceRestrictions +
        "  " + (food_sourceRestrictions.size() > 0 ? "AND " : "") + "Long_Desc LIKE "
        "    CONCAT('%', :terms, '%') "
        "ORDER BY Description ASC LIMIT 500";

    runSearchQuery(searchQuery, resultSet);

    if (ui.lstCategories->isItemSelected(ui.lstCategories->item(0))) {

      searchQuery =
          "SELECT Composite_Id AS Id, 'Composite Food' AS Type, Description"
          "   FROM composite_food "
          "WHERE " + composite_sourceRestrictions +
          "  " + (composite_sourceRestrictions.size() > 0 ? "AND " : "") + "Description LIKE "
          "   CONCAT('%', :terms, '%') "
          "ORDER BY Description ASC LIMIT 500";

      runSearchQuery(searchQuery, resultSet);
    }

    for (std::set<Result>::const_iterator i = resultSet.begin(); i != resultSet.end(); ++i) {
      emit newResult(i->id, i->type, i->description);
    }
  }
}

void FoodSearchControl::setDatabase(const QSqlDatabase& db)
{
  static const QString COMPOUND_NAME = "(Composite Foods)";

  this->db = db;

  ui.lstCategories->clear();
  categoryToGroupID.clear();

  ui.lstCategories->addItem(COMPOUND_NAME);
  categoryToGroupID.insert(COMPOUND_NAME, "");

  QSqlQuery catQuery = db.exec("SELECT * FROM group_description ORDER BY FdGrp_Desc ASC;");

  int idField = catQuery.record().indexOf("FdGrp_Cd");
  int nameField = catQuery.record().indexOf("FdGrp_Desc");

  while (catQuery.next()) {
    ui.lstCategories->addItem(catQuery.value(nameField).toString());
    categoryToGroupID.insert(catQuery.value(nameField).toString(),
                             catQuery.value(idField).toString());
  }

  ui.lstCategories->selectAll();
}

void FoodSearchControl::runSearchQuery(const QString& queryText, std::set<Result>& results) const
{
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

  while (query.next()) {
    results.insert(Result(query.value(idField).toInt(),
                          query.value(typeField).toString(),
                          query.value(descField).toString()));
  }
}

