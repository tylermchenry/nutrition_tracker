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
#include <QMultiMap>
#include <QVariant>

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
class FoodCollection : virtual public Food
{
  public:

    // Definitions to make this class cacheable with DataCache
    typedef int cache_key_type;
    typedef FoodCollection cache_object_type;
    static const bool cache_strong = false;

    struct ContainedTypes {
      enum ContainedType {
          SingleFood,
          CompositeFood,
          Template, /* TODO: Find a more logical place for this */
      };
      static ContainedType fromHumanReadable(const QString& str);
      static QString toHumanReadable(ContainedType src);
    };

    static QSharedPointer<FoodCollection> createFoodCollection
      (const QString& name = "");

    static QSharedPointer<FoodCollection> createFoodCollection
      (const QSharedPointer<FoodCollection>& copy);

    static QMultiMap<QString, QPair<ContainedTypes::ContainedType, int> >
      getFoodsForUser(int userId);

    static QMultiMap<QString, QPair<ContainedTypes::ContainedType, int> >
      getFoodsForUser(int userId, ContainedTypes::ContainedType type);

    virtual ~FoodCollection() {};

    virtual FoodComponent addComponent(const FoodAmount& foodAmount) = 0;

    virtual QVector<FoodComponent> addComponents
      (const QVector<FoodAmount>& components) = 0;

    virtual FoodComponent changeComponentAmount
      (const FoodComponent& component, const FoodAmount& amount) = 0;

    virtual void removeComponent(const FoodComponent& component) = 0;

    virtual void clearComponents() = 0;

    virtual QVector<FoodComponent> merge
      (const QSharedPointer<const FoodCollection>& otherFC) = 0;

    virtual void replaceWith
      (const QSharedPointer<const FoodCollection>& otherFC) = 0;

  protected:

    virtual QSharedPointer<Food>
      getCanonicalSharedPointer() const;

    virtual QSharedPointer<FoodCollection>
      getCanonicalSharedPointerToCollection() const = 0;

    virtual int getFoodCollectionId() const = 0;

    virtual void setComponents(const QList<FoodComponent>& components) = 0;

    virtual bool needsToBeReSaved() const = 0;

    virtual int getNextTemporaryId() const = 0;

    virtual const QMap<int, FoodComponent>& getRawComponents() const = 0;

    virtual const QMap<int, int>& getNewIds() const = 0;

    virtual const QSet<int>& getRemovedIds() const = 0;

  private:

    static int nextCollectionId;

    friend class FoodCollectionImpl;
};

Q_DECLARE_METATYPE(FoodCollection::ContainedTypes::ContainedType);

#endif /* FOOD_COLLECTION_H_ */



