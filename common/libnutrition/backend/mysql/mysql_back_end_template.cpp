#include "mysql_back_end.h"
#include "libnutrition/data/data_cache.h"
#include "libnutrition/data/impl/template_impl.h"
#include <QVariant>
#include <QDebug>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlRecord>
#include <QtSql/QSqlField>
#include <QtSql/QSqlError>
#include <stdexcept>

QSharedPointer<Template> MySQLBackEnd::loadTemplate(int id)
{
  QSqlQuery query(db);

  query.prepare("SELECT template.Template_Id, template.User_Id, template.Description, "
                "       template_link.TemplateLink_Id, template_link.Contained_Type, "
                "       template_link.Contained_Id, template_link.Includes_Refuse, "
                "       template_link.Magnitude, template_link.Unit, "
                "       template_link.IntrafoodOrder "
                "FROM"
                "        template "
                "   LEFT JOIN template_link "
                "        ON template.Template_Id = template_link.Template_Id "
                "WHERE template.Template_Id = :id "
                "ORDER BY template_link.IntrafoodOrder ASC");

  query.bindValue(":id", id);

  if (query.exec()) {
    qDebug() << "Executed query: " << query.executedQuery();
    return createTemplateFromQueryResults(query);
  } else {
    qDebug() << "Query failed: " << query.lastError();
    return QSharedPointer<Template>();
  }
}

QMultiMap<QString, int> MySQLBackEnd::loadTemplateNamesForUser(int userId)
{
  QSqlQuery query(db);
  QMultiMap<QString, int> foods;

  query.prepare("SELECT template.Template_Id, template.Description "
                 "FROM   template "
                 "WHERE  template.User_Id = :userId");

  query.bindValue(":userId", userId);

  if (query.exec()) {
    while (query.next()) {
      const QSqlRecord& record = query.record();
      foods.insert(record.field("Description").value().toString(),
                   record.field("Template_Id").value().toInt());
    }
  } else {
    qDebug() << "Query failed: " << query.lastError();
  }

  return foods;
}

void MySQLBackEnd::storeTemplate(const QSharedPointer<Template>& templ)
{
  // TODO: Replace
  templ->saveToDatabase();
}

QSharedPointer<Template> MySQLBackEnd::createTemplateFromQueryResults
  (QSqlQuery& query)
{
  QSharedPointer<TemplateImpl> food_impl;

  if (query.first()) {

    const QSqlRecord& record = query.record();
    int id = record.field("Template_Id").value().toInt();

    food_impl = QSharedPointer<TemplateImpl>
    (new TemplateImpl(id,
                      record.field("Description").value().toString(),
                      record.field("User_Id").value().toInt()));
    DataCache<Template>::getInstance().insert(id, food_impl);
  }

  if (food_impl && query.isActive()) {
    query.seek(-1); // Reset to before first record
    food_impl->setComponents
      (createComponentsFromQueryResults
         (query, food_impl, "TemplateLink_Id", "IntrafoodOrder"));
    if (food_impl->needsToBeReSaved()) {
      food_impl->saveToDatabase();
    }
  }

  return food_impl;
}

