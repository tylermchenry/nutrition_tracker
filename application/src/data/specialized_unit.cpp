/*
 * specialized_unit.cpp
 * Part of nutrition_tracker
 *
 *  Created on: Jun 25, 2010
 *      Author: Tyler McHenry <tyler@nerdland.net>
 */

#include "specialized_unit.h"
#include "single_food.h"
#include <QVariant>
#include <QDebug>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlRecord>
#include <QtSql/QSqlField>
#include <QtSql/QSqlError>

QMap<QPair<int, int>, QWeakPointer<const SpecializedUnit> >
   SpecializedUnit::specializedUnitCache;

QSharedPointer<const SpecializedUnit>
  SpecializedUnit::getSpecializedUnit(int foodId, int sequence)
{
  QSqlDatabase db = QSqlDatabase::database("nutrition_db");
  QSqlQuery query(db);

  QPair<int, int> cacheIndex = qMakePair(foodId, sequence);

  if (specializedUnitCache[cacheIndex]) {
    return specializedUnitCache[cacheIndex].toStrongRef();
  }

  query.prepare("SELECT Food_Id, Seq, Amount, Msre_Desc, Gm_Wgt "
                 " FROM weight WHERE Food_Id=:foodId AND Seq=:sequence "
                 " ORDER BY Seq LIMIT 1");
  query.bindValue(":foodId", foodId);
  query.bindValue(":sequence", sequence);

  if (query.exec() && query.first()) {
    return createSpecializedUnitFromRecord(query.record());
  } else {
    return QSharedPointer<const SpecializedUnit>();
  }
}

QVector<QSharedPointer<const SpecializedUnit> >
  SpecializedUnit::getAllSpecializedUnitsForFoodId(int foodId)
{
  QSqlDatabase db = QSqlDatabase::database("nutrition_db");
  QSqlQuery query(db);

  query.prepare("SELECT Food_Id, Seq, Amount, Msre_Desc, Gm_Wgt "
                 " FROM weight WHERE Food_Id=:foodId ORDER BY Seq");
  query.bindValue(":foodId", foodId);

  if (query.exec()) {
    return createSpecializedUnitsFromQueryResults(query);
  } else {
    return QVector<QSharedPointer<const SpecializedUnit> >();
  }
}

QSharedPointer<const SpecializedUnit>
  SpecializedUnit::createSpecializedUnitFromRecord(const QSqlRecord& record)
{
  if (!record.isEmpty()) {
    int foodId = record.field("Food_Id").value().toInt();
    int sequence = record.field("Seq").value().toInt();
    QPair<int, int> cacheIndex = qMakePair(foodId, sequence);
    if (!specializedUnitCache[cacheIndex]) {
      QSharedPointer<const SpecializedUnit> spUnit
      (new SpecializedUnit(foodId, sequence,
                           record.field("Msre_Desc").value().toString(),
                           record.field("Gm_Wgt").value().toDouble()));
      specializedUnitCache[cacheIndex] = spUnit;
      return spUnit;
    } else {
      return specializedUnitCache[cacheIndex].toStrongRef();
    }
  } else {
    return QSharedPointer<const SpecializedUnit>();
  }
}

QVector<QSharedPointer<const SpecializedUnit> >
  SpecializedUnit::createSpecializedUnitsFromQueryResults(QSqlQuery& query)
{
  QVector<QSharedPointer<const SpecializedUnit> > spUnits;

  while (query.next()) {
    spUnits.push_back(createSpecializedUnitFromRecord(query.record()));
  }

  return spUnits;
}

SpecializedUnit::SpecializedUnit
  (int foodId, int sequence, const QString& name, double amountInGrams)
  : foodId(foodId), sequence(sequence), name(name), amountInGrams(amountInGrams)
{
}

SpecializedUnit::~SpecializedUnit()
{
}

QSharedPointer<Food> SpecializedUnit::getFood() const
{
  return SingleFood::getSingleFood(foodId);
}
