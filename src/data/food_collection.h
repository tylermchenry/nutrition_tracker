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
#include "food_component.h"

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

    virtual QVector<FoodAmount> getComponentAmounts() const;

    virtual QSet<FoodComponent> getComponents() const;

    virtual QMap<QString, NutrientAmount> getNutrients() const;

    virtual void addComponent(const FoodAmount& foodAmount);

    virtual void addComponents(const QVector<FoodAmount>& components);

    virtual void removeComponent(const FoodComponent& component);

    virtual void clearComponents();

    virtual void saveToDatabase();

  protected:

    FoodCollection(const QString& id, const QString& name,
                    const QSet<FoodComponent>& components,
                    double weightAmount, double volumeAmount,
                    double quantityAmount, double servingAmount);

    // Derived classes call to replace a component that has been newly
    // saved to the database with one that has the DB-assigned ID.
    void replaceComponent(const FoodComponent& oldComponent,
                             const FoodComponent& newComponent);

    inline const QSet<int>& getRemovedIds() const { return removedIds; }

    inline void clearRemovedIds() { removedIds.clear(); }

    virtual QSharedPointer<Food> getCanonicalSharedPointer();

    virtual QSharedPointer<const Food> getCanonicalSharedPointer() const;

    static QSet<FoodComponent> createComponentsFromQueryResults
      (QSqlQuery& query, const QString& componentIdField,
       const QString& componentOrderField = "Order");

  private:

    FoodCollection(int id, const QString& name,
                    const QSet<FoodComponent>& components,
                    double weightAmount, double volumeAmount,
                    double quantityAmount, double servingAmount);

    int id;
    QSet<FoodComponent> components;

    // Temporary IDs for components not yet saved to the database
    int nextTemporaryId;

    // Maps DB-assigned IDs to old temporary IDs, in case removeComponent is passed
    // and old temporary ID after saving to the DB
    QMap<int, int> newIds;

    QSet<int> removedIds;

    static QMap<QString, NutrientAmount>& mergeNutrients
      (QMap<QString, NutrientAmount>& nutrients,
       const QMap<QString, NutrientAmount>& newNutrients);

    static int nextCollectionId;

    static QMap<int, QWeakPointer<FoodCollection> > foodCollectionCache;
};


#endif /* FOOD_COLLECTION_H_ */



