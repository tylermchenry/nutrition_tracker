#include "mysql_back_end.h"
#include <QVariant>
#include <QDebug>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlRecord>
#include <QtSql/QSqlField>
#include <QtSql/QSqlError>
#include <stdexcept>

QList<MySQLBackEnd::SearchResult> MySQLBackEnd::searchFoods
  (const SearchRequest& request)
{
  QSqlQuery query(db);

  int userId = request.userId;

  userId = (userId >= 0) ? userId : User::getLoggedInUser()->getId();

  QString food_sourceRestrictions;

  if (!request.sourceUSDA) {
    food_sourceRestrictions = "Entry_Src != 'USDA'";
  }

  if (!request.sourceImport) {
    if (food_sourceRestrictions != "") food_sourceRestrictions += " AND ";
    food_sourceRestrictions = "Entry_Src != 'Import'";
  }

  if (!request.sourceOthers) {
    if (food_sourceRestrictions != "") food_sourceRestrictions += " AND ";
    food_sourceRestrictions = " (Entry_Src != 'Custom' OR User_Id == "
        + QString::number(userId) + ")";
  }

  if (!request.sourceSelf) {
    if (food_sourceRestrictions != "") food_sourceRestrictions += " AND ";
    food_sourceRestrictions = " (Entry_Src != 'Custom' OR User_Id != "
        + QString::number(userId) + ")";
  }

  QString composite_sourceRestrictions;

  if (!request.includeExpired) {
    composite_sourceRestrictions = "(ExpiryDate IS NULL OR ExpiryDate > DATE(NOW()))";
  }

  QString categoryRestrictions;

  for (QList<QSharedPointer<const Group> >::const_iterator i = request.excludeGroups.begin();
       i != request.excludeGroups.end(); ++i)
  {
   if (categoryRestrictions.size() > 0) {
     categoryRestrictions += " AND ";
   }
   categoryRestrictions += "food_description.FdGrp_Cd != " + (*i)->getId();
  }

  // For whatever incredibly lame reason, the QMYSQL driver does not seem to
  // support UNION queries, which this should logically be. If I try that,
  // it stops returning results after it hits the first result from the
  // second table in the UNION. Instead, this search is executed as two
  // separate single-table queries, and then the results are "union'ed"
  // client-side by inserting them into a QMap, sorted by description.

  QMap<QString, SearchResult> resultSet;

  if (request.searchSingleFoods) {
    QString searchQuery =
        "SELECT Food_Id AS Id, 'Food' AS Type, Long_Desc AS Description,"
        "       NULL AS CreationDate, NULL AS ExpiryDate "
        "  FROM food_description "
        "WHERE " + categoryRestrictions +
        "  " + (categoryRestrictions.size() > 0 ? "AND " : "") + food_sourceRestrictions +
        "  " + (food_sourceRestrictions.size() > 0 ? "AND " : "") + "Long_Desc LIKE "
        "    CONCAT('%', :terms, '%') "
        "ORDER BY Description ASC LIMIT " + QString::number(request.maxResults);

    runSearchQuery(searchQuery, request.searchTerms, resultSet);
  }

  if (request.searchComposites) {

    QString searchQuery =
        "SELECT Composite_Id AS Id, 'CompositeFood' AS Type, Description, "
        "       CreationDate, ExpiryDate"
        "   FROM composite_food "
        "WHERE IsNonce = 0 AND " + composite_sourceRestrictions +
        "  " + (composite_sourceRestrictions.size() > 0 ? "AND " : "") + "Description LIKE "
        "   CONCAT('%', :terms, '%') "
        "ORDER BY Description ASC LIMIT " + QString::number(request.maxResults);

    runSearchQuery(searchQuery, request.searchTerms, resultSet);
  }

  // TODO: Allow searching Templates

  QList<SearchResult> resultList = resultSet.values();

  if (resultList.size() > request.maxResults) {
    resultList.erase(resultList.begin() + request.maxResults, resultList.end());
  }

  return resultList;
}

void MySQLBackEnd::runSearchQuery
  (const QString& queryText, const QString& terms,
   QMap<QString, SearchResult>& results) const
{
  QSqlQuery query(db);

  qDebug() << "Query is: " << queryText;

  if (!query.prepare(queryText)) {
    qDebug() << "Prepare failed: " << query.lastError().text();
    return;
  }

  query.bindValue(":terms", terms);

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
                   SearchResult
                     (query.value(descField).toString() +
                        CompositeFood::generateExpirySuffix
                          (query.value(creationField).toDate(),
                           query.value(expiryField).toDate()),
                      FoodTypes::fromContainedType(query.value(typeField).toString()),
                      query.value(idField).toInt()));
  }
}
