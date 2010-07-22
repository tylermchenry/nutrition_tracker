/*
 * food_server.h
 *
 *  Created on: Jul 21, 2010
 *      Author: tyler
 */

#ifndef FOOD_SERVER_H_
#define FOOD_SERVER_H_

#include "libnutrition/data/food.h"
#include <QString>
#include <QSet>

class DataLoadResponse; // forward decl

class FoodLoadResponseObjects
{
  public:

    void addFood(const QSharedPointer<const Food>& food);
    void addFoods(const QVector<QSharedPointer<const Food> >& foods);
    void addFoods(const QList<QSharedPointer<const Food> >& foods);

    inline bool isEmpty() const { return foods.isEmpty(); }
    bool contains(const QSharedPointer<const Food>& food) const;

    QList<QSharedPointer<const Food> > getFoods() const;
    QSet<QString> getFoodIds() const;

    DataLoadResponse& serialize(DataLoadResponse& resp) const;

  private:

    QSet<QString> foodIds;
    QList<QSharedPointer<const Food> > foods;
};


#endif /* FOOD_SERVER_H_ */
