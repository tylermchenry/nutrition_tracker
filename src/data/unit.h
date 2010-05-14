/*
 * unit.h
 *
 *  Created on: May 14, 2010
 *      Author: tmchenry
 */

#ifndef UNIT_H_
#define UNIT_H_

#include <QString>

class Unit
{
  public:

    struct Dimensions {
      enum Dimension {
          Weight,
          Volume,
          Quantity,
          Serving
      };
      static const Dimension PREFERRED_DIMENSION = Dimensions::Weight;
    };

    static const Unit* getPreferredUnit
      (Dimensions::Dimension dimension = Dimensions::PREFERRED_DIMENSION);

    Unit(const QString& abbrevation = "");

    virtual ~Unit();

    inline QString getAbbreviation() const { return abbreviation; }

    inline QString getName() const { return name; }

    inline QString getNameAndAbbreviation() const
      { return name + " (" + abbreviation + ")"; }

    inline Dimensions::Dimension getDimension() const { return dimension; }

    // Conversion factor is what you need to multiply an amount in unit by
    // in order to get the amount in terms of otherUnit.
    double getConversionFactor(const Unit* otherUnit) const;

  private:

    QString abbreviation;
    QString name;
    Dimensions::Dimension dimension;
    double basicConversionFactor;

    static const Unit* getBasicUnit(Dimensions::Dimension dimension);

};

#endif /* UNIT_H_ */
