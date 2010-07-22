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

    void addFood(const QSharedPointer<Food>& food);
    void addFoods(const QVector<QSharedPointer<Food> >& foods);
    void addFoods(const QList<QSharedPointer<Food> >& foods);

    inline bool isEmpty() const { return foods.isEmpty(); }
    bool contains(const QSharedPointer<Food>& food);

    QList<QSharedPointer<Food> > getFoods() const;

    DataLoadResponse& serialize(DataLoadResponse& resp) const;

  private:

    QSet<QString> foodIds;
    QList<QSharedPointer<Food> > foods;
};


#endif /* FOOD_SERVER_H_ */
