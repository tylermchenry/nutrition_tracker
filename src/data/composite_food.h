/*
 * composite_food.h
 *
 *  Created on: May 14, 2010
 *      Author: tmchenry
 */

#ifndef COMPOSITE_FOOD_H_
#define COMPOSITE_FOOD_H_

#include "food.h"

class CompositeFood: public Food
{
  public:

    struct ContainedTypes {
      enum ContainedType {
          SingleFood,
          CompositeFood
      };
      static ContainedType fromHumanReadable(const QString& str);
      static QString toHumanReadable(ContainedType src);
    };

    static QSharedPointer<CompositeFood> getCompositeFood(int id);

    static QSharedPointer<CompositeFood> createCompositeFoodFromQueryResults
      (QSqlQuery& query);

    virtual ~CompositeFood();

    virtual QVector<FoodAmount> getComponents() const;

    virtual QMap<QString, NutrientAmount> getNutrients() const;

    virtual void addComponent(const FoodAmount& foodAmount);

    virtual void addComponents(const QVector<FoodAmount>& components);

    virtual void saveToDatabase();

  protected:

    CompositeFood(const QString& id, const QString& name,
                    const QVector<FoodAmount>& components,
                    double weightAmount, double volumeAmount,
                    double quantityAmount, double servingAmount);

    virtual QSharedPointer<Food> getCanonicalSharedPointer();

    virtual QSharedPointer<const Food> getCanonicalSharedPointer() const;

    static QVector<FoodAmount> createComponentsFromQueryResults
      (QSqlQuery& query);

  private:

    CompositeFood(int id, const QString& name,
                    const QVector<FoodAmount>& components,
                    double weightAmount, double volumeAmount,
                    double quantityAmount, double servingAmount);

    int id;
    QVector<FoodAmount> components;

    static QMap<QString, NutrientAmount>& mergeNutrients
      (QMap<QString, NutrientAmount>& nutrients,
       const QMap<QString, NutrientAmount>& newNutrients);

    static QMap<int, QWeakPointer<CompositeFood> > compositeFoodCache;
};

#endif /* COMPOUND_FOOD_H_ */
