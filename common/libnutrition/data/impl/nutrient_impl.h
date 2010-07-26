/*
 * nutrient.h - Data object for a single nutrient
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

#ifndef NUTRIENT_IMPL_H_
#define NUTRIENT_IMPL_H_

#include "libnutrition/data/nutrient.h"
#include "libnutrition/data/unit.h"
#include <QString>
#include <QMap>
#include <QSharedPointer>

/* A nutrient object describes a particular sort of nutrient that may be found
 * in a food. Nutrients are divided into several categories, mainly for display
 * purposes. All nutrients function in the same way.
 *
 * Each nutrient has a unique ID, a name, a category, and a standard unit. The
 * standard unit is the unit (usually of weight) in which a food's contained
 * amount of this nutrient is described. E.g. for Protein, this is grams, but
 * for Vitamin A it is IU, and for Calories it is kilocalories. In the database,
 * all single food entries simply store the numeric amount of each nutrient that
 * they contain, and the unit for the amount is implied to be the standard unit
 * for that nutrient. So it is impossible for nutrient amounts to be stored in
 * the database in anything other than their standard unit.
 *
 * Some nutrients also have a USDA reference daily intake (RDI), which is a
 * numeric amount in terms of the standard unit that the USDA recommends an
 * average person consume in a given day. For nutrients that do not have RDIs
 * defined by the USDA, this value will be 0.
 */
class NutrientImpl : virtual public Nutrient
{
  public:

    using Nutrient::Categories;

    NutrientImpl
      (const QString& id, const QString& name, Categories::Category category,
       const QSharedPointer<const Unit>& standardUnit, double rdi);

    virtual ~NutrientImpl();

    virtual inline QString getId() const { return id; }

    virtual inline QString getName() const { return name; }

    virtual inline Categories::Category getCategory() const
      { return category; }

    virtual inline QSharedPointer<const Unit> getStandardUnit() const
      { return standardUnit; }

    virtual inline double getRDI() const { return rdi; }

    virtual double getDefaultCalorieDensity() const; // in kcal/g

    virtual NutrientData serialize() const;

    // Only in the impl, for the benefit of the back end
    static void setAllLoaded()
      { Nutrient::setAllLoaded(); }

  private:

    QString id;
    QString name;
    Categories::Category category;
    QSharedPointer<const Unit> standardUnit;
    double rdi; // RDI in terms of standard unit
};

#endif /* NUTRIENT_IMPL_H_ */
