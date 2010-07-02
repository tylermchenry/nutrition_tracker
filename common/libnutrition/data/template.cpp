#include "template.h"
#include "impl/template_impl.h"
#include "data_cache.h"
#include "nutrient_amount.h"
#include "food_amount.h"
#include "single_food.h"
#include "composite_food.h"
#include <QVariant>
#include <QDebug>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlRecord>
#include <QtSql/QSqlField>
#include <QtSql/QSqlError>

int Template::tempId = -1;

QSharedPointer<Template> Template::createNewTemplate
  (const QSharedPointer<const Template>& copy)
{
  QSharedPointer<Template> food(new TemplateImpl(copy));
  DataCache<Template>::getInstance().insert(food->getTemplateId(), food);
  return food;
}

QSharedPointer<Template> Template::getTemplate(int id)
{
  QSqlDatabase db = QSqlDatabase::database("nutrition_db");
  QSqlQuery query(db);

  if (DataCache<Template>::getInstance().contains(id)) {
    return DataCache<Template>::getInstance().get(id);
  }

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

QSharedPointer<Template> Template::createTemplateFromQueryResults
  (QSqlQuery& query)
{
  QSharedPointer<Template> food;

  if (query.first()) {

    const QSqlRecord& record = query.record();
    int id = record.field("Template_Id").value().toInt();

    if (!DataCache<Template>::getInstance().contains(id)) {
      food = QSharedPointer<Template>
        (new TemplateImpl(id,
                          record.field("Description").value().toString(),
                          record.field("User_Id").value().toInt()));
      DataCache<Template>::getInstance().insert(id, food);
    } else {
      return DataCache<Template>::getInstance().get(id);
    }

  }

  if (food && query.isActive()) {
    query.seek(-1); // Reset to before first record
    food->setComponents
      (createComponentsFromQueryResults
         (query, food, "TemplateLink_Id", "IntrafoodOrder"));
    if (food->needsToBeReSaved()) {
      food->saveToDatabase();
    }
  }

  return food;
}

QMultiMap<QString, int> Template::getFoodsForUser(int userId)
{
  QSqlDatabase db = QSqlDatabase::database("nutrition_db");
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

QSharedPointer<Food> Template::getCanonicalSharedPointer() const
{
  return DataCache<Template>::getInstance().get(getTemplateId());
}

