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
  if (!templ) {
    throw std::logic_error("Attempted to store NULL template");
  }

  QSharedPointer<TemplateImpl> templ_impl = templ.dynamicCast<TemplateImpl>();

  QSqlQuery query(db);

  qDebug() << "Saving template to database.";

  // This needs to work either for a new food or an update to an existing food

  query.prepare("INSERT INTO template "
                "  (Template_Id, User_Id, Description) "
                "VALUES "
                "  (:id, :user_id, :name) "
                "ON DUPLICATE KEY UPDATE"
                "  User_Id=:user_id2, Description=:name2");

  query.bindValue(":id", (templ->getTemplateId() >= 0 ?
    QVariant(templ->getTemplateId()) : QVariant(QVariant::Int)));

  query.bindValue(":user_id", templ->getOwnerId());
  query.bindValue(":user_id2", templ->getOwnerId());
  query.bindValue(":name", templ->getName());
  query.bindValue(":name2", templ->getName());

  if (!query.exec()) {
    qDebug() << "Query error: " << query.lastError();
    throw std::runtime_error("Failed to save template to database.");
  }

  if (templ->getTemplateId() < 0) {
    int newId = query.lastInsertId().toInt();
    DataCache<Template>::getInstance().changeKey(templ->getTemplateId(), newId);
    qDebug() << "Assigned real ID " << newId << " to template with temp ID "
              << templ->getTemplateId();
    templ_impl->setTemplateId(newId);
  }

  QSet<int> removedLinkIds = templ_impl->getRemovedIds();

  for (QSet<int>::const_iterator i = removedLinkIds.begin(); i != removedLinkIds.end(); ++i)
  {
    query.prepare("DELETE FROM template_link WHERE TemplateLink_Id=:linkId");

    query.bindValue(":linkId", *i);

    if (!query.exec()) {
      qDebug() << "Failed to delete removed template item: " << query.lastError();
      return;
    }
  }

  QList<FoodComponent> components = templ->getComponents();
  for (QList<FoodComponent>::const_iterator i = components.begin(); i != components.end(); ++i)
  {
    if (!query.prepare("INSERT INTO template_link "
        "  (TemplateLink_Id, Template_Id, Contained_Type, "
        "   Contained_Id, Includes_Refuse, Magnitude, Unit, IntrafoodOrder) "
        "VALUES "
        "  (:linkId, :templateId, :containedType, "
        "   :containedId, :includesRefuse, :magnitude, :unit, :order) "
        "ON DUPLICATE KEY UPDATE "
        "  Includes_Refuse=:includesRefuse2, Magnitude=:magnitude2, "
        "  Unit=:unit2, IntrafoodOrder=:order2")) {
      qDebug() << "Failed to prepare query: " << query.lastError();
    }

    query.bindValue(":linkId", i->getId() >= 0 ? QVariant(i->getId()) : QVariant());

    query.bindValue(":templateId", templ->getTemplateId());

    if (!i->getFoodAmount().isDefined()) continue;

    QSharedPointer<const Food> food = i->getFoodAmount().getFood();

    QSharedPointer<const SingleFood> singleFood;
    QSharedPointer<const CompositeFood> compositeFood;

    FoodCollection::ContainedTypes::ContainedType containedType;
    int containedId;

    if ((singleFood = food.dynamicCast<const SingleFood>()) != NULL) {
      containedType = FoodCollection::ContainedTypes::SingleFood;
      containedId = singleFood->getSingleFoodId();
    } else if ((compositeFood = food.dynamicCast<const CompositeFood>()) != NULL) {
      containedType = FoodCollection::ContainedTypes::CompositeFood;
      containedId = compositeFood->getCompositeFoodId();
    } else {
      continue;
    }

    query.bindValue(":containedType", FoodCollection::ContainedTypes::toHumanReadable(containedType));
    query.bindValue(":containedId", containedId);

    query.bindValue(":includesRefuse", i->getFoodAmount().includesRefuse());
    query.bindValue(":magnitude", i->getFoodAmount().getAmount());
    query.bindValue(":unit", i->getFoodAmount().getUnit()->getAbbreviation());
    query.bindValue(":order", i->getOrder());

    query.bindValue(":includesRefuse2", i->getFoodAmount().includesRefuse());
    query.bindValue(":magnitude2", i->getFoodAmount().getAmount());
    query.bindValue(":unit2", i->getFoodAmount().getUnit()->getAbbreviation());
    query.bindValue(":order2", i->getOrder());

    if (!query.exec()) {
      qDebug() << "Failed to save " << food->getName() << " to template: " << query.lastError();
    } else {
      if (i->getId() < 0) {
        int newId = query.lastInsertId().toInt();
        qDebug() << "Assigned real ID " << newId
                  << " to food component with temp ID " << i->getId();
        templ_impl->replaceComponent
          (*i, FoodComponent(templ_impl->getCanonicalSharedPointerToCollection(),
                             newId, i->getFoodAmount(), i->getOrder()));
      }
    }
  }
}

void MySQLBackEnd::deleteTemplate(const QSharedPointer<Template>& templ)
{
  if (!templ) {
    throw std::logic_error("Attempted to delete NULL template");
  } else if (templ->getTemplateId() < 0) {
    // Silently ignore requests to delete temporaries; it's simpler to do that
    // than to force the client to check whether a food is temporary or not
    // every time it wants to delete something.
    return;
  }

  QSharedPointer<TemplateImpl> templ_impl = templ.dynamicCast<TemplateImpl>();

  QSqlQuery query(db);

  query.prepare("DELETE FROM template_link WHERE Template_Id=:templateId");
  query.bindValue(":templateId", templ->getTemplateId());

  if (!query.exec()) {
    qDebug() << "Failed to delete components of " << templ->getName() << ": "
              << query.lastError();
    return;
  }

  query.prepare("DELETE FROM template WHERE Template_Id=:templateId");
  query.bindValue(":templateId", templ->getTemplateId());

  if (!query.exec()) {
    qDebug() << "Failed to delete template " << templ->getName() << ": "
              << query.lastError();
    return;
  }

  templ_impl->clearComponents();
  DataCache<Template>::getInstance().remove(templ->getTemplateId());
}

void MySQLBackEnd::deleteTemplate(int id)
{
  deleteTemplate(Template::getTemplate(id));
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

