/*
 * nutrient.cpp
 *
 *  Created on: May 14, 2010
 *      Author: tmchenry
 */

#include "nutrient.h"
#include "impl/nutrient_impl.h"
#include "data_cache.h"
#include "libnutrition/backend/back_end.h"
#include <QDebug>

const QString Nutrient::CALORIES_NAME = "Calories";
const QString Nutrient::FAT_NAME = "Total Fat";
const QString Nutrient::CARBOHYDRATE_NAME = "Total Carbohydrate";
const QString Nutrient::PROTEIN_NAME = "Protein";
const QString Nutrient::ALCOHOL_NAME = "Alcohol";

QString (Nutrient::* const Nutrient::cache_get_sort_key)() const = &Nutrient::getName;

QVector<QSharedPointer<const Nutrient> > Nutrient::all;
QMap<QString, QSharedPointer<const Nutrient> > Nutrient::allByName;
QMap<Nutrient::Categories::Category, QVector<QSharedPointer<const Nutrient> > >
  Nutrient::allByCategory;

QSharedPointer<const Nutrient> Nutrient::getNutrientByName(const QString& name)
{
  if (allByName.empty()) {
    getAllNutrients();
  }

  if (allByName.contains(name)) {
    return allByName[name];
  } else {
    return QSharedPointer<const Nutrient>();
  }
}

QSharedPointer<const Nutrient> Nutrient::getNutrient(const QString& id)
{
  if (DataCache<Nutrient>::getInstance().contains(id)) {
    return DataCache<Nutrient>::getInstance().get(id);
  } else {
    return BackEnd::getBackEnd()->loadNutrient(id);
  }
}

QVector<QSharedPointer<const Nutrient> > Nutrient::getAllNutrients()
{
  if (all.isEmpty()) {
    BackEnd::getBackEnd()->loadAllNutrients();
    setAllLoaded();
  }

  return all;
}

QVector<QSharedPointer<const Nutrient> > Nutrient::getAllNutrients
  (Categories::Category category)
{
  if (allByCategory[category].isEmpty()) {
    QList<QSharedPointer<Nutrient> > mutable_byCategory =
      BackEnd::getBackEnd()->loadAllNutrients(category);

    for (QList<QSharedPointer<Nutrient> >::const_iterator i = mutable_byCategory.begin();
         i != mutable_byCategory.end(); ++i)
    {
      allByCategory[category].append(*i);
    }
  }

  return allByCategory[category];
}

void Nutrient::setAllLoaded()
{
  // TODO: Make this method return a QList so this conversion is unnecessary
  all = DataCache<Nutrient>::getInstance().getAll().toVector();

  allByName.clear();
  allByCategory.clear();

  for (QVector<QSharedPointer<const Nutrient> >::const_iterator i = all.begin();
      i != all.end(); ++i)
  {
    if (*i) {
      allByName.insert((*i)->getName(), *i);
      allByCategory[(*i)->getCategory()].append(*i);
    }
  }
}

Nutrient::Categories::Category Nutrient::Categories::fromHumanReadable(const QString& str)
{
  QString lowerStr = str.toLower();

  if (lowerStr == "energy")     return Categories::Energy;
  if (lowerStr == "basic")      return Categories::Basic;
  if (lowerStr == "vitamin")    return Categories::Vitamin;
  if (lowerStr == "mineral")    return Categories::Mineral;
  if (lowerStr == "additional") return Categories::Additional;
  if (lowerStr == "hidden")     return Categories::Hidden;

  qDebug() << "Bad category: " << str;
  throw std::range_error("String does not describe a Category.");
}

QString Nutrient::Categories::toHumanReadable(Category cat)
{
  switch (cat) {
    case Energy:     return "Energy";
    case Basic:      return "Basic";
    case Vitamin:    return "Vitamin";
    case Mineral:    return "Mineral";
    case Additional: return "Additional";
    case Hidden:     return "Hidden";
    default:         throw std::range_error("Category enumeration out of range.");
  }
}
