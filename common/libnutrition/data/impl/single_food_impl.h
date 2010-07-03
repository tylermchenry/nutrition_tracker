/*
 * single_food.h - Data object for a single, indivisible food
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

#ifndef SINGLE_FOOD_IMPL_H_
#define SINGLE_FOOD_IMPL_H_

#include "food_impl.h"
#include "libnutrition/data/single_food.h"
#include "libnutrition/data/food_collection.h"
#include "libnutrition/data/unit.h"
#include "libnutrition/data/group.h"
#include "libnutrition/data/specialized_unit.h"
#include <QMap>
#include <QMultiMap>
#include <QSet>

/* A single food is an indivisible food with its own inherent nutrition
 * information. In this case "indivisible" does not necessarily mean that it is
 * a raw agricultural product (e.g. an apple), but simply that no breakdown of
 * this food into other foods is known to the user. So commercially-produced
 * foods which have USDA nutrition labels, and for which the user obviously
 * cannot know the exact amounts or nature of their ingredients (e.g. a can of
 * Progresso minestrone soup or a McDonald's Big Mac) are considered single
 * foods. This is in contrast to home-made foods where there is no such
 * unified nutrition label, but the ingredients are known; those are composite
 * foods.
 *
 * The database is pre-populated with many single foods available from the
 * public USDA nutrition database. It is expected that the user will enter
 * additional single foods by transcribing the nutrition labels from
 * commercially-produced foods that they consume.
 *
 * Each food has a unique ID, a name (which need not be unique, but should be,
 * since the GUI will only display the name, and not the ID), a set of nutrient
 * amounts, a group, and a source. The source is USDA (if it came from the USDA
 * public database), Import (if it was brought in from a third-party database),
 * or Custom if the user transcribed the nutrition label directly into the
 * tracker.
 */
class SingleFoodImpl : public FoodImpl, virtual public SingleFood
{
  public:

    using SingleFood::EntrySources;

    SingleFoodImpl(int id, const QString& name, int ownerId,
                     EntrySources::EntrySource entrySource,
                     const QSharedPointer<const Group>& group,
                     double percentRefuse, const QString& refuseDescription,
                     const QMap<QString, NutrientAmount>& nutrients,
                     double weightAmount, double volumeAmount,
                     double quantityAmount, double servingAmount,
                     double energyDensityFat, double energyDensityCarbohydrate,
                     double energyDensityProtien, double energyDensityAlcohol);

    // Default or "Copy" constructor. If a food is passed in to copy, the
    // attributes of the food object are copied, but the constructed food
    // is still assigned a new, temporary ID.
    SingleFoodImpl(const QSharedPointer<const SingleFood>& copy =
                     QSharedPointer<const SingleFood>());

    virtual ~SingleFoodImpl();

    virtual inline int getSingleFoodId() const { return id; }

    virtual inline EntrySources::EntrySource getEntrySource() const
      { return entrySource; }

    virtual inline QSharedPointer<const Group> getGroup() const
      { return group; }

    virtual inline double getPercentRefuse() const
      { return percentRefuse; }

    virtual inline QString getRefuseDescription() const
      { return refuseDescription; }

    virtual QMap<QString, NutrientAmount> getNutrients() const;

    virtual NutrientAmount getCaloriesFromNutrientId
      (const QString& nutrId) const;

    virtual inline QVector<QSharedPointer<const SpecializedUnit> >
      getAllSpecializedUnits() const
        { return SpecializedUnit::getAllSpecializedUnitsForFoodId(id); }

    virtual inline QSharedPointer<const SpecializedUnit>
      getSpecializedUnit(int sequence) const
        { return SpecializedUnit::getSpecializedUnit(id, sequence); }

    virtual void setEntrySource(EntrySources::EntrySource source);

    virtual void setGroup(const QSharedPointer<const Group>& group);

    virtual void setNutrient(const NutrientAmount& nutrientAmount);

    virtual void saveToDatabase();

    virtual void deleteFromDatabase();

    // The following methods are only in the Impl class and are not accessible
    // through the interface. They exist for the benefit of the back ends:

    inline void setSingleFoodId(int id)
      { this->id = id; }

    inline const QMap<QString, NutrientAmount>& getRawNutrients() const
      { return nutrients; }

    inline const QSet<QString>& getModifiedNutrients() const
      { return modifiedNutrients; }

    inline void clearModifiedNutrients()
      { modifiedNutrients.clear(); }

  protected:

    virtual inline const QMap<QString, double>&
      getCalorieDensities() const
        { return calorieDensities; }

  private:

    int id;
    EntrySources::EntrySource entrySource;
    QSharedPointer<const Group> group;
    double percentRefuse;
    QString refuseDescription;
    QMap<QString, NutrientAmount> nutrients;
    QSet<QString> modifiedNutrients;
    QMap<QString, double> calorieDensities;

    void setCalorieDensity(const QString& nutrientName, double density);

    void sanityCheckCalorieDensities();
};

#endif /* SINGLE_FOOD_IMPL_H_ */
