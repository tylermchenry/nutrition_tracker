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

#ifndef SINGLE_FOOD_H_
#define SINGLE_FOOD_H_

#include "food.h"
#include "unit.h"
#include "group.h"
#include "specialized_unit.h"
#include <QMap>
#include <QMultiMap>
#include <QSet>

class SingleFoodData; // forward decl

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
class SingleFood : virtual public Food
{
  public:

    // Definitions to make this class cacheable with DataCache
    typedef int cache_key_type;
    typedef SingleFood cache_object_type;
    static const bool cache_strong = false;

    struct EntrySources {
      enum EntrySource {
          USDA,
          Import,
          Custom
      };
      static EntrySource fromHumanReadable(const QString& str);
      static QString toHumanReadable(EntrySource src);
    };

    static QSharedPointer<SingleFood> createNewFood
      (const QSharedPointer<const SingleFood>& copy =
       QSharedPointer<const SingleFood>());

    static QSharedPointer<SingleFood> getSingleFood(int id);

    static QMultiMap<QString, int> getFoodsForUser(int userId);

    virtual int getSingleFoodId() const = 0;

    virtual EntrySources::EntrySource getEntrySource() const = 0;

    virtual QSharedPointer<const Group> getGroup() const = 0;

    virtual double getPercentRefuse() const = 0;

    virtual QString getRefuseDescription() const = 0;

    virtual QVector<QSharedPointer<const SpecializedUnit> >
      getAllSpecializedUnits() const = 0;

    virtual QSharedPointer<const SpecializedUnit>
      getSpecializedUnit(int sequence) const = 0;

    virtual void setEntrySource(EntrySources::EntrySource source) = 0;

    virtual void setGroup(const QSharedPointer<const Group>& group) = 0;

    virtual void setNutrient(const NutrientAmount& nutrientAmount) = 0;

    virtual FoodData& serialize(FoodData& fdata) const = 0;

    virtual SingleFoodData serialize() const = 0;

  protected:

    virtual QSharedPointer<Food> getCanonicalSharedPointer() const;

    virtual const QMap<QString, double>& getCalorieDensities() const = 0;

  private:

    static int tempId;

    friend class SingleFoodImpl;
};

#endif /* SINGLE_FOOD_H_ */
