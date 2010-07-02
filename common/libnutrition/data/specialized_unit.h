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

    struct SpecializedUnitIdTuple
    {
      int foodId;
      int sequence;

      SpecializedUnitIdTuple(int fid, int s);

      bool operator< (const SpecializedUnitIdTuple& rhs) const;
    };

    // Definitions to make this class cacheable with DataCache
    typedef SpecializedUnitIdTuple cache_key_type;
    typedef const SpecializedUnit cache_object_type;
    typedef SpecializedUnitIdTuple cache_sort_key_type;
    static SpecializedUnitIdTuple (SpecializedUnit::* const cache_get_sort_key)() const;
    static const bool cache_strong = true;

    static QSharedPointer<const SpecializedUnit>
      getSpecializedUnit(int foodId, int sequence);

    static QVector<QSharedPointer<const SpecializedUnit> >
      getAllSpecializedUnitsForFoodId(int foodId);

    virtual ~SpecializedUnit() {};

    virtual int getFoodId() const = 0;

    virtual QSharedPointer<Food> getFood() const = 0;

    virtual int getSequence() const = 0;

    inline SpecializedUnitIdTuple getSpecializedUnitIdTuple() const
      { return SpecializedUnitIdTuple(getFoodId(), getSequence()); }

    virtual QString getName() const = 0;

    virtual QString getAbbreviation() const = 0;

    virtual QString getNameAndAbbreviation() const = 0;

    virtual FoodAmount getBaseAmount() const = 0;

  private:

    static QSharedPointer<const SpecializedUnit>
      createSpecializedUnitFromRecord(const QSqlRecord& record);

    static QVector<QSharedPointer<const SpecializedUnit> >
      createSpecializedUnitsFromQueryResults(QSqlQuery& query);

    static QMap<QPair<int, int>, QWeakPointer<const SpecializedUnit> >
      specializedUnitCache;

    friend class SpecializedUnitImpl;
};

#endif /* SPECIALIZED_UNIT_H_ */
