#include "template_impl.h"
#include "libnutrition/data/nutrient_amount.h"
#include "libnutrition/data/food_amount.h"
#include "libnutrition/data/single_food.h"
#include "libnutrition/data/composite_food.h"
#include <QVariant>
#include <QDebug>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlRecord>
#include <QtSql/QSqlField>
#include <QtSql/QSqlError>

TemplateImpl::TemplateImpl(int id, const QString& name, int ownerId,
                      const QList<FoodComponent>& components)
  : FoodCollectionImpl
      ("TEMPLATE_" + QString::number(id), name, ownerId, components,
       0, 0, 0, 0),
    id(id)
{
  if (needsToBeReSaved()) {
    saveToDatabase();
  }
}

TemplateImpl::TemplateImpl(const QSharedPointer<const Template>& copy)
  : FoodCollectionImpl("TEMPLATE_" + QString::number(tempId), copy),
    id(tempId--)
{
  qDebug() << "Created new template with temporary ID " << id;
}

TemplateImpl::~TemplateImpl()
{
}

void TemplateImpl::saveToDatabase()
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

void TemplateImpl::deleteFromDatabase()
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
