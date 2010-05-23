/*
 * composite_food.h
 *
 *  Created on: May 14, 2010
 *      Author: tmchenry
 */

#ifndef COMPOSITE_FOOD_H_
#define COMPOSITE_FOOD_H_

#include "food_collection.h"

class CompositeFood: public FoodCollection
{
  public:

    static QSharedPointer<CompositeFood> getCompositeFood(int id);

    static QSharedPointer<CompositeFood> createCompositeFoodFromQueryResults
      (QSqlQuery& query);

    virtual ~CompositeFood();

    virtual void saveToDatabase();

  protected:

    virtual QSharedPointer<Food> getCanonicalSharedPointer();

    virtual QSharedPointer<const Food> getCanonicalSharedPointer() const;

  private:

    CompositeFood(int id, const QString& name,
                    const QVector<FoodAmount>& components,
                    double weightAmount, double volumeAmount,
                    double quantityAmount, double servingAmount);

    int id;

    static QMap<int, QWeakPointer<CompositeFood> > compositeFoodCache;
};

#endif /* COMPOUND_FOOD_H_ */
