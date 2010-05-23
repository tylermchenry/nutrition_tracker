/*
 * composite_food.cpp
 *
 *  Created on: May 14, 2010
 *      Author: tmchenry
 */

#include "composite_food.h"
#include "data/nutrient_amount.h"
#include "data/food_amount.h"
#include "data/single_food.h"
#include <QVariant>
#include <QDebug>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlRecord>
#include <QtSql/QSqlField>
#include <QtSql/QSqlError>

QMap<int, QWeakPointer<CompositeFood> > CompositeFood::compositeFoodCache;

QSharedPointer<CompositeFood> CompositeFood::getCompositeFood(int id)
{
  QSqlDatabase db = QSqlDatabase::database("nutrition_db");
  QSqlQuery query(db);

  if (compositeFoodCache[id]) {
    return compositeFoodCache[id].toStrongRef();
  }

  query.prepare("SELECT composite_food.Composite_Id, composite_food.Description, "
                "       composite_food.Weight_g, composite_food.Volume_floz, "
                "       composite_food.Quantity, composite_food.Servings, "
                "       composite_food_link.Contained_Type, composite_food_link.Contained_Id, "
                "       composite_food_link.Magnitude, units.Unit, units.Type, "
                "       units.Name, units.Factor "
                "FROM"
                "        composite_food "
                "   JOIN composite_food_link "
                "        ON composite_food.Composite_Id = composite_food_link.Composite_Id "
                "   JOIN units "
                "        ON composite_food_link.Unit = units.Unit "
                "WHERE composite_food.Composite_Id = :id");
  query.bindValue(":id", id);

  if (query.exec()) {
    qDebug() << "Executed query: " << query.executedQuery();
    return createCompositeFoodFromQueryResults(query);
  } else {
    return QSharedPointer<CompositeFood>();
  }
}

QSharedPointer<CompositeFood> CompositeFood::createCompositeFoodFromQueryResults
  (QSqlQuery& query)
{
  QVector<FoodAmount> components = createComponentsFromQueryResults(query);

  if (query.first()) {
    const QSqlRecord& record = query.record();
    int id = record.field("Composite_Id").value().toInt();
    if (!compositeFoodCache[id]) {
      QSharedPointer<CompositeFood> food
      (new CompositeFood(id,
                         record.field("Description").value().toString(),
                         components,
                         record.field("Weight_g").value().toDouble(),
                         record.field("Volume_floz").value().toDouble(),
                         record.field("Quantity").value().toDouble(),
                         record.field("Servings").value().toDouble()));
      compositeFoodCache[id] = food;
      return food;
    } else {
      return compositeFoodCache[id].toStrongRef();
    }
  } else {
    return QSharedPointer<CompositeFood>();
  }
}

CompositeFood::CompositeFood(const QString& id, const QString& name,
                             const QVector<FoodAmount>& components,
                             double weightAmount, double volumeAmount,
                             double quantityAmount, double servingAmount)
  : Food(id, name, weightAmount, volumeAmount,
         quantityAmount, servingAmount), id(-1), components(components)
{
}

CompositeFood::CompositeFood(int id, const QString& name,
                             const QVector<FoodAmount>& components,
                             double weightAmount, double volumeAmount,
                             double quantityAmount, double servingAmount)
  : Food("COMPOSITE_" + QString::number(id), name, weightAmount, volumeAmount,
         quantityAmount, servingAmount), id(id), components(components)
{
  qDebug() << "Created composite ID " << id << " named " << name << ", amounts: "
           << weightAmount << " g, " << volumeAmount << " fl oz "
           << quantityAmount << "qty, " << servingAmount << " srv.";

  qDebug() << "Component Amounts:";

  for (QVector<FoodAmount>::const_iterator i = components.begin(); i != components.end(); ++i)
  {
    qDebug() << i->getAmount() << " " << i->getUnit()->getNameAndAbbreviation()
             << " of " << i->getFood()->getName();
  }
}

CompositeFood::~CompositeFood()
{
}

QVector<FoodAmount> CompositeFood::getComponents() const
{
  return components;
}

QMap<QString, NutrientAmount> CompositeFood::getNutrients() const
{
  // TODO: Intelligently cache instead of recomputing each time

  QMap<QString, NutrientAmount> nutrients;

  for (QVector<FoodAmount>::const_iterator i = components.begin();
       i != components.end(); ++i)
  {
    mergeNutrients(nutrients, i->getScaledNutrients());
  }

  return nutrients;
}

void CompositeFood::addComponent(const FoodAmount& foodAmount)
{
  components.push_back(foodAmount);
}

void CompositeFood::addComponents(const QVector<FoodAmount>& components)
{
  this->components += components;
}

void CompositeFood::saveToDatabase()
{
}

QSharedPointer<Food> CompositeFood::getCanonicalSharedPointer()
{
  return compositeFoodCache[id].toStrongRef();
}

QSharedPointer<const Food> CompositeFood::getCanonicalSharedPointer() const
{
  return compositeFoodCache[id].toStrongRef();
}

QVector<FoodAmount> CompositeFood::createComponentsFromQueryResults
  (QSqlQuery& query)
{
  QVector<FoodAmount> components;

  while (query.next()) {

    const QSqlRecord& record = query.record();

    int containedId = record.field("Contained_Id").value().toInt();

    ContainedTypes::ContainedType type =
      ContainedTypes::fromHumanReadable(record.field("Contained_Type").value().toString());

    QSharedPointer<const Food> containedFood;

    if (type == ContainedTypes::SingleFood) {
      containedFood = SingleFood::getSingleFood(containedId);
    } else if (type == ContainedTypes::CompositeFood) {
      // TODO: Check for infinite recursion and throw exception
      containedFood = CompositeFood::getCompositeFood(containedId);
    }

    if (containedFood != NULL) {
      components.push_back(FoodAmount
        (containedFood, record.field("Magnitude").value().toDouble(),
         Unit::createUnitFromRecord(record)));
    }
  }

  return components;
}

QMap<QString, NutrientAmount>& CompositeFood::mergeNutrients
  (QMap<QString, NutrientAmount>& nutrients,
   const QMap<QString, NutrientAmount>& newNutrients)
{
  for (QMap<QString, NutrientAmount>::const_iterator i = newNutrients.begin();
       i != newNutrients.end(); ++i)
  {
    nutrients[i.key()] += i.value();
  }

  return nutrients;
}

CompositeFood::ContainedTypes::ContainedType CompositeFood::ContainedTypes::fromHumanReadable
  (const QString& str)
{
  QString lowerStr = str.toLower();

  if (lowerStr == "food")          return ContainedTypes::SingleFood;
  if (lowerStr == "compositefood") return ContainedTypes::CompositeFood;

  throw std::range_error("String does not describe a contained type.");
}

QString CompositeFood::ContainedTypes::toHumanReadable(ContainedType src)
{
  switch (src) {
    case SingleFood:    return "Food";
    case CompositeFood: return "CompositeFood";
    default:     throw std::range_error("ContainedType enumeration out of range.");
  }
}
