/*
 * food.h
 *
 *  Created on: May 14, 2010
 *      Author: tmchenry
 */

#ifndef FOOD_H_
#define FOOD_H_

#include <QVector>
#include <QMap>
#include <QString>
#include <QSharedPointer>
#include "data/food_amount.h"
#include "data/unit.h"

class Food
{
  public:

    virtual ~Food();

    inline QString getId() const { return id; }

    inline QString getName() const { return name; }

    QList<Unit::Dimensions::Dimension> getValidDimensions() const;

    FoodAmount getBaseAmount
      (Unit::Dimensions::Dimension dimension = Unit::Dimensions::PREFERRED_DIMENSION) const;

    void setBaseAmount(double amount, const QSharedPointer<const Unit>& unit);

    virtual QVector<FoodAmount> getComponents() const;

    virtual QMap<QString, NutrientAmount> getNutrients() const = 0;

    void setName(const QString& name);

    virtual void saveToDatabase() = 0;

  protected:

    Food(const QString& id, const QString& name, double weightAmount,
         double volumeAmount, double quantityAmount, double servingAmount);

    virtual QSharedPointer<Food> getCanonicalSharedPointer() = 0;

    virtual QSharedPointer<const Food> getCanonicalSharedPointer() const = 0;

    void bindBaseAmount
      (QSqlQuery& query, const QString& placeholder, Unit::Dimensions::Dimension dimension) const;

  private:

    QString id;
    QString name;
    QMap<Unit::Dimensions::Dimension, double> baseAmounts;

};

#endif /* FOOD_H_ */
