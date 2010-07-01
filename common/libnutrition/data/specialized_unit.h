/*
 * specialized_unit.h
 * Part of nutrition_tracker
 *
 *  Created on: Jun 25, 2010
 *      Author: Tyler McHenry <tyler@nerdland.net>
 */

#ifndef SPECIALIZED_UNIT_H_
#define SPECIALIZED_UNIT_H_

#include <QVector>
#include <QMap>
#include <QPair>
#include <QSharedPointer>
#include <QWeakPointer>
#include <QtSql/QSqlQuery>
#include "food_amount.h"
#include "unit.h"

class SpecializedUnit
{
  public:

    static QSharedPointer<const SpecializedUnit>
      getSpecializedUnit(int foodId, int sequence);

    static QVector<QSharedPointer<const SpecializedUnit> >
      getAllSpecializedUnitsForFoodId(int foodId);

    virtual ~SpecializedUnit();

    inline int getFoodId() const
      { return foodId; }

    QSharedPointer<Food> getFood() const;

    inline int getSequence() const
      { return sequence; }

    inline QString getName() const
      { return name; }

    inline QString getAbbreviation() const
      { return QString::number(amountInGrams, 'f', 0) + " g"; }

    inline QString getNameAndAbbreviation() const
      { return getName() + " (" + getAbbreviation() + ")"; }

    inline FoodAmount getBaseAmount() const
      { return FoodAmount(getFood(), amountInGrams, Unit::getUnit("g")); }

  private:

    static QSharedPointer<const SpecializedUnit>
      createSpecializedUnitFromRecord(const QSqlRecord& record);

    static QVector<QSharedPointer<const SpecializedUnit> >
      createSpecializedUnitsFromQueryResults(QSqlQuery& query);

    SpecializedUnit(int foodId, int sequence, const QString& name,
                      double amountInGrams);

    int foodId;
    int sequence;
    QString name;
    double amountInGrams;

    static QMap<QPair<int, int>, QWeakPointer<const SpecializedUnit> >
      specializedUnitCache;

};

#endif /* SPECIALIZED_UNIT_H_ */
