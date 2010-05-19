/*
 * single_food.h
 *
 *  Created on: May 14, 2010
 *      Author: tmchenry
 */

#ifndef SINGLE_FOOD_H_
#define SINGLE_FOOD_H_

#include "food.h"
#include "unit.h"
#include "group.h"
#include <QMap>
#include <QWeakPointer>

class SingleFood: public Food
{
  public:

    struct EntrySources {
      enum EntrySource {
          USDA,
          Import,
          Custom
      };
      static EntrySource fromHumanReadable(const QString& str);
      static QString toHumanReadable(EntrySource src);
    };

    static QSharedPointer<SingleFood> getSingleFood(int id);

    static QSharedPointer<SingleFood> createSingleFoodFromQueryResults(QSqlQuery& query);

    virtual ~SingleFood();

    inline int getSingleFoodId() const { return id; }

    inline EntrySources::EntrySource getEntrySource() const { return entrySource; }

    inline QSharedPointer<const Group> getGroup() const { return group; }

    virtual QMap<QString, NutrientAmount> getNutrients() const;

  protected:

    virtual QSharedPointer<Food> getCanonicalSharedPointer();

    virtual QSharedPointer<const Food> getCanonicalSharedPointer() const;

  private:

    SingleFood(int id, const QString& name, EntrySources::EntrySource entrySource,
                const QSharedPointer<const Group>& group,
                const QMap<QString, NutrientAmount>& nutrients,
                double weightAmount, double volumeAmount,
                double quantityAmount, double servingAmount);

    int id;
    EntrySources::EntrySource entrySource;
    QSharedPointer<const Group> group;
    QMap<QString, NutrientAmount> nutrients;

    static QMap<QString, QSharedPointer<const Unit> > createNutrientsFromQueryResults
      (QSqlQuery& query);

    static QMap<int, QWeakPointer<SingleFood> > singleFoodCache;
};

#endif /* SINGLE_FOOD_H_ */
