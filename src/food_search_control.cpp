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

    QString compound_sourceRestrictions;

    if (!ui.chkOld->isChecked()) {
      // TODO: Implement when old markin gis implemented
      // compound_sourceRestrictions = "Old == 0";
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

    QString searchQuery =
        " (SELECT Food_Id AS Id, 'Food' AS Type, Long_Desc AS Description"
        "   FROM food_description"
        " WHERE " + categoryRestrictions +
        "   " + (categoryRestrictions.size() > 0 ? "AND " : "") + food_sourceRestrictions +
        "   " + (food_sourceRestrictions.size() > 0 ? "AND " : "") + "Long_Desc LIKE "
        "     CONCAT('%', :terms1, '%')) ";

    if (ui.lstCategories->isItemSelected(ui.lstCategories->item(0))) {
      searchQuery +=
          " UNION (SELECT Compound_Id AS Id, 'Compound Food' AS Type, Description"
          "   FROM compound_food"
          " WHERE " + compound_sourceRestrictions +
          "   " + (compound_sourceRestrictions.size() > 0 ? "AND " : "") + "Description LIKE "
          "    CONCAT('%', :terms2, '%')) ";
    }

    searchQuery += " ORDER BY Description ASC LIMIT 500;";

    qDebug() << "Composed query: " << searchQuery;

    if (!query.prepare(searchQuery)) {
      qDebug() << "Prepare failed: " << query.lastError().text();
      return;
    }

    query.bindValue(":terms1", ui.txtTerms->text());

    if (ui.lstCategories->isItemSelected(ui.lstCategories->item(0))) {
      query.bindValue(":terms2", ui.txtTerms->text());
    }

    if (!query.exec()) {
      qDebug() << "Exec failed"  << query.lastError().text();
      return;
    }

    qDebug() << "Ran query: " << query.executedQuery();

    int idField = query.record().indexOf("Id");
    int descField = query.record().indexOf("Description");

    while (query.next()) {
      emit newResult(query.value(idField).toInt(), "Food",
                     query.value(descField).toString());
    }

  }
}

void FoodSearchControl::setDatabase(const QSqlDatabase& db)
{
  static const QString COMPOUND_NAME = "(Compound Foods)";

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
