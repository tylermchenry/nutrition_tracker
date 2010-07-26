/*
 * unit.h - Data object for a unit of measurement
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

#ifndef UNIT_H_
#define UNIT_H_

#include <QMap>
#include <QString>
#include <QVector>
#include <QSharedPointer>
#include <QWeakPointer>

class UnitData; // forward decl

/* A unit object represents a unit of measurement such as a gram, a fluid ounce,
 * a serving, or a kilocalorie.
 *
 * Each unit has a name and an abbreviation. The abbreviation is unique, and
 * functions as the unique identifier of the unit. Names are not necessarily
 * unique (in terms of database constraints), but obviously they logically ought
 * to be.
 *
 * Units of the same dimension can compute conversion factors between one
 * another, so that an Amount object denominated in a given unit can be easily
 * converted to be denominated in any other unit of the same dimension. E.g. you
 * could convert an Amount of 1.2 lbs to 544.31 g, but you couldn't convert
 * 1.2 lbs to Liters, servings, or kilocalories since those units describe
 * completely different dimensions, and the conversion would require more
 * information than just the units involved.
 *
 * Each dimension has a "base unit", and each unit object contains a conversion
 * factor for converting to the base unit. Conversion factors between two units
 * X and Y are computed by multiplying the X->Base conversion factor by the
 * Base->Y conversion factor (which is the inverse of the Y->Base factor). Base
 * units are arbitrary, but were chosen to be gram and fluid ounce for weight
 * and volume in order to minimize the magnitude of the conversion factors.
 *
 * The Weight, Volume, and Energy dimensions are self-explanatory. The IU
 * dimension is used for some vitamins. It is really a weight dimension, but its
 * meaning is defined in terms of the vitamin it is describing, so it isn't
 * possible to convert IU to grams, since one would need to know "IU of what?",
 * so IU and weight need to be separate dimensions to prevent this conversion.
 * The Serving dimension has only one unit, the serving, which describes the
 * serving size (either USDA-defined or user-defined) of a particular food. The
 * Quantity dimension is for discrete, countable foods, e.g. there might be
 * nutrition data for a Quantity of 28 almonds, but it wouldn't make sense to
 * describe a Quantity of milk, since amounts of milk are continuous (you would
 * instead describe a Volume, Weight, or Serving of milk).
 */
class Unit
{
  public:

    // Definitions to make this class cacheable with DataCache
    typedef QString cache_key_type;
    typedef const Unit cache_object_type;
    typedef QString cache_sort_key_type;
    static QString (Unit::* const cache_get_sort_key)() const;
    static const bool cache_strong = true;

    struct Dimensions {
      enum Dimension {
          Weight,
          Volume,
          Quantity,
          Serving,
          Energy,
          IU
      };
      static const Dimension PREFERRED_DIMENSION = Dimensions::Weight;
      static const QVector<Dimension>& getAllDimensions();
      static Dimension fromHumanReadable(const QString& str);
      static QString toHumanReadable(Dimension dim);
    };

    static QSharedPointer<const Unit> getPreferredUnit
      (Dimensions::Dimension dimension = Dimensions::PREFERRED_DIMENSION);

    static QSharedPointer<const Unit> getUnit(const QString& abbreviation);

    static QVector<QSharedPointer<const Unit> > getAllUnits();

    static QVector<QSharedPointer<const Unit> > getAllUnits
      (Dimensions::Dimension dimension);

    virtual ~Unit() {};

    virtual QString getAbbreviation() const = 0;

    virtual QString getName() const = 0;

    virtual QString getNameAndAbbreviation() const = 0;

    virtual Dimensions::Dimension getDimension() const = 0;

    // Conversion factor is what you need to multiply an amount in unit by
    // in order to get the amount in terms of otherUnit.
    virtual double getConversionFactor
      (const QSharedPointer<const Unit>& otherUnit =
        QSharedPointer<const Unit>()) const = 0;

    virtual UnitData serialize() const = 0;

    virtual bool operator==(const Unit& rhs) const = 0;

  protected:

    virtual double getBasicConversionFactor() const = 0;

  private:

    static QVector<QSharedPointer<const Unit> > all;
    static QMap<Dimensions::Dimension, QVector<QSharedPointer<const Unit> > > allByDimension;
    static QMap<Dimensions::Dimension, QSharedPointer<const Unit> > basicUnits;

    static QSharedPointer<const Unit> getBasicUnit
      (Dimensions::Dimension dimension);

    friend class UnitImpl;
};

#endif /* UNIT_H_ */
