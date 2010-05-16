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

    static QSharedPointer<const SingleFood> getSingleFood(int id);

    static QSharedPointer<const SingleFood> createSingleFoodFromQueryResults(QSqlQuery& query);

    virtual ~SingleFood();

    inline int getSingleFoodId() const { return id; }

    inline EntrySources::EntrySource getEntrySource() const { return entrySource; }

    virtual QMap<QString, NutrientAmount> getNutrients() const;

  private:

    SingleFood(int id, const QString& name, EntrySources::EntrySource entrySource,
                const QMap<QString, NutrientAmount>& nutrients,
                double weightAmount, double volumeAmount,
                double quantityAmount, double servingAmount);

    int id;
    EntrySources::EntrySource entrySource;
    QMap<QString, NutrientAmount> nutrients;

    static QMap<QString, QSharedPointer<const Unit> > createNutrientsFromQueryResults
      (QSqlQuery& query);
};

#endif /* SINGLE_FOOD_H_ */
