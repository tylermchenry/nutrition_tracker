/*
 * food_collection.h - Data object for an arbitrary collection of foods
 *
 * This file is part of Nutrition Tracker.
 *
 * Nutrition Tracker is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Nutrition Tracker is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Nutrition Tracker.  If not, see <http://www.gnu.org/licenses/>.
 *
 * Copyright © 2010 Tyler McHenry <tyler@nerdland.net>
 */

#ifndef FOOD_COLLECTION_H_
#define FOOD_COLLECTION_H_

#include "food.h"
#include "food_component.h"

/* A FoodCollection is an arbitrary collection of single and composite foods
 * which is not necessarily treated logically as its own first-class food
 * (but classes derived from FoodCollection may be treated this way).
 *
 * This class centralizes code dealing with management of the "components" that
 * make up the collection. This logic is common to "raw" collections (objects
 * of dynamic type FoodCollection), meals, and composite foods (which both
 * derive from FoodCollection).
 *
 * This class can compute and return the combined nutrient amounts for all
 * components.
 *
 * Additionally, this class keeps track of removed components (for use by
 * database-backed derived classes later, for DELETE queries), and takes care
 * of generating temporary identifiers for components that are newly added
 * and not yet in the database. When such a component is inserted into the
 * database, the derived class' saveToDatabase method is expected to notify
 * the FoodCollection parent class so that it can update its mapping of
 * previously-used temporary IDs to actual IDs. This mapping is maintained
 * in case external code passes in (e.g. to removeComponent()) a component
 * object that is still carrying the now-defunct temporary ID.
 */
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

    static QSharedPointer<FoodCollection> createFoodCollection
      (const QSharedPointer<FoodCollection>& copy);

    virtual ~FoodCollection();

    virtual QVector<FoodAmount> getComponentAmounts() const;

    virtual QSet<FoodComponent> getComponents() const;

    virtual QMap<QString, NutrientAmount> getNutrients() const;

    virtual NutrientAmount getCaloriesFromNutrientId
      (const QString& nutrId) const;

    virtual FoodComponent addComponent(const FoodAmount& foodAmount);

    virtual QVector<FoodComponent> addComponents
      (const QVector<FoodAmount>& components);

    virtual FoodComponent changeComponentAmount(const FoodComponent& component, const FoodAmount& amount);

    virtual void removeComponent(const FoodComponent& component);

    virtual void clearComponents();

    virtual QVector<FoodComponent> merge
      (const QSharedPointer<const FoodCollection>& otherFC);

    virtual void replaceWith
      (const QSharedPointer<const FoodCollection>& otherFC);

    virtual void saveToDatabase();

  protected:

    FoodCollection(const QString& id, const QString& name,
                    const QSet<FoodComponent>& components,
                    double weightAmount, double volumeAmount,
                    double quantityAmount, double servingAmount);

    FoodCollection(const QString& id, const QString& name,
                   double weightAmount, double volumeAmount,
                   double quantityAmount, double servingAmount);

    // If the subclass chooses not to pass components to the constructor,
    // they can be set later through this method
    void setComponents(const QSet<FoodComponent>& components);

    // Derived classes call to replace a component that has been newly
    // saved to the database with one that has the DB-assigned ID.
    void replaceComponent(const FoodComponent& oldComponent,
                             const FoodComponent& newComponent);

    inline const QSet<int>& getRemovedIds() const { return removedIds; }

    inline void clearRemovedIds() { removedIds.clear(); }

    virtual QSharedPointer<Food> getCanonicalSharedPointer();

    virtual QSharedPointer<const Food> getCanonicalSharedPointer() const;

    virtual QSharedPointer<FoodCollection>
      getCanonicalSharedPointerToCollection();

    virtual QSharedPointer<const FoodCollection>
      getCanonicalSharedPointerToCollection() const;

    static QSet<FoodComponent> createComponentsFromQueryResults
      (QSqlQuery& query,
       const QSharedPointer<FoodCollection>& containingCollection,
       const QString& componentIdField,
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

    // Maps DB-assigned IDs to old temporary IDs, in case removeComponent is
    // passed and old temporary ID after saving to the DB
    QMap<int, int> newIds;

    QSet<int> removedIds;

    static QMap<QString, NutrientAmount>& mergeNutrients
      (QMap<QString, NutrientAmount>& nutrients,
       const QMap<QString, NutrientAmount>& newNutrients);

    static int nextCollectionId;

    static QMap<int, QWeakPointer<FoodCollection> > foodCollectionCache;
};


#endif /* FOOD_COLLECTION_H_ */



