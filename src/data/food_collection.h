/*
 * food_collection.h
 * Part of nutrition_tracker
 *
 *  Created on: May 22, 2010
 *      Author: Tyler McHenry <tyler@nerdland.net>
 */

#ifndef FOOD_COLLECTION_H_
#define FOOD_COLLECTION_H_

#include "food.h"

class FoodCollection: public Food
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

    static QSharedPointer<FoodCollection> createFoodCollection
      (const QString& name = "");

    virtual ~FoodCollection();

    virtual QVector<FoodAmount> getComponents() const;

    virtual QMap<int, FoodAmount> getComponentsWithIndices() const;

    virtual QMap<QString, NutrientAmount> getNutrients() const;

    virtual void addComponent(const FoodAmount& foodAmount);

    virtual void addComponents(const QVector<FoodAmount>& components);

    virtual void removeComponent(int index);

    virtual void clearComponents();

    virtual void saveToDatabase();

  protected:

    FoodCollection(const QString& id, const QString& name,
                    const QVector<FoodAmount>& components,
                    double weightAmount, double volumeAmount,
                    double quantityAmount, double servingAmount);

    virtual QSharedPointer<Food> getCanonicalSharedPointer();

    virtual QSharedPointer<const Food> getCanonicalSharedPointer() const;

    static QVector<FoodAmount> createComponentsFromQueryResults
      (QSqlQuery& query);

  private:

    FoodCollection(int id, const QString& name,
                    const QVector<FoodAmount>& components,
                    double weightAmount, double volumeAmount,
                    double quantityAmount, double servingAmount);

    int id;
    QMap<int, FoodAmount> components;
    int nextIndex;

    static QMap<QString, NutrientAmount>& mergeNutrients
      (QMap<QString, NutrientAmount>& nutrients,
       const QMap<QString, NutrientAmount>& newNutrients);

    static int nextCollectionId;

    static QMap<int, QWeakPointer<FoodCollection> > foodCollectionCache;
};


#endif /* FOOD_COLLECTION_H_ */



