#include "template.h"
#include "data/nutrient_amount.h"
#include "data/food_amount.h"
#include "data/single_food.h"
#include "data/composite_food.h"
#include <QVariant>
#include <QDebug>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlRecord>
#include <QtSql/QSqlField>
#include <QtSql/QSqlError>

QMap<int, QWeakPointer<Template> > Template::templateCache;

int Template::tempId = -1;

QSharedPointer<Template> Template::createNewTemplate
  (const QSharedPointer<const Template>& copy)
{
  QSharedPointer<Template> food(new Template(copy));
  templateCache[food->getTemplateId()] = food;
  return food;
}

QSharedPointer<Template> Template::getTemplate(int id)
{
  QSqlDatabase db = QSqlDatabase::database("nutrition_db");
  QSqlQuery query(db);

  if (templateCache[id]) {
    return templateCache[id].toStrongRef();
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

    if (!templateCache[id]) {
      food = QSharedPointer<Template>
        (new Template(id,
                      record.field("Description").value().toString(),
                      record.field("User_Id").value().toInt()));
      templateCache[id] = food;
    } else {
      return templateCache[id].toStrongRef();
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

Template::Template(int id, const QString& name, int ownerId,
                      const QList<FoodComponent>& components)
  : FoodCollection("TEMPLATE_" + QString::number(id), name, ownerId, components,
                   0, 0, 0, 0),
    id(id)
{
  if (needsToBeReSaved()) {
    saveToDatabase();
  }
}

Template::Template(const QSharedPointer<const Template>& copy)
  : FoodCollection("TEMPLATE_" + QString::number(tempId), copy),
    id(tempId--)
{
  qDebug() << "Created new template with temporary ID " << id;
}

Template::~Template()
{
}

void Template::saveToDatabase()
{
  QSqlDatabase db = QSqlDatabase::database("nutrition_db");
  QSqlQuery query(db);

  qDebug() << "Saving template to database.";

  // This needs to work either for a new food or an update to an existing food

  query.prepare("INSERT INTO template "
                "  (Template_Id, User_Id, Description) "
                "VALUES "
                "  (:id, :user_id, :name) "
                "ON DUPLICATE KEY UPDATE"
                "  User_Id=:user_id2, Description=:name2");

  query.bindValue(":id", (id >= 0 ? QVariant(id) : QVariant(QVariant::Int)));

  query.bindValue(":user_id", getOwnerId());
  query.bindValue(":user_id2", getOwnerId());
  query.bindValue(":name", getName());
  query.bindValue(":name2", getName());

  if (!query.exec()) {
    qDebug() << "Query error: " << query.lastError();
    throw std::runtime_error("Failed to save template to database.");
  }

  if (id < 0) {
    int newId = query.lastInsertId().toInt();
    templateCache[newId] = templateCache[id];
    templateCache.remove(id);
    qDebug() << "Assigned real ID " << newId << " to template with temp ID " << id;
    id = newId;
  }

  QSet<int> removedLinkIds = getRemovedIds();

  for (QSet<int>::const_iterator i = removedLinkIds.begin(); i != removedLinkIds.end(); ++i)
  {
    query.prepare("DELETE FROM template_link WHERE TemplateLink_Id=:linkId");

    query.bindValue(":linkId", *i);

    if (!query.exec()) {
      qDebug() << "Failed to delete removed template item: " << query.lastError();
      return;
    }
  }

  QList<FoodComponent> components = getComponents();
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

    query.bindValue(":templateId", id);

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
        replaceComponent
        (*i, FoodComponent(getCanonicalSharedPointerToCollection(),
                           newId, i->getFoodAmount(), i->getOrder()));
      }
    }
  }
}

void Template::deleteFromDatabase()
{
  if (id < 0) return;

  QSqlDatabase db = QSqlDatabase::database("nutrition_db");
  QSqlQuery query(db);

  query.prepare("DELETE FROM template_link WHERE Template_Id=:templateId");
  query.bindValue(":templateId", id);

  if (!query.exec()) {
    qDebug() << "Failed to delete components of " << getName() << ": "
              << query.lastError();
    return;
  }

  query.prepare("DELETE FROM template WHERE Template_Id=:templateId");
  query.bindValue(":templateId", id);

  if (!query.exec()) {
    qDebug() << "Failed to delete template " << getName() << ": "
              << query.lastError();
    return;
  }

  clearComponents();
  templateCache[id].clear();
}

QSharedPointer<Food> Template::getCanonicalSharedPointer() const
{
  return templateCache[id].toStrongRef();
}

