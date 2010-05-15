/*
 * unit.h
 *
 *  Created on: May 14, 2010
 *      Author: tmchenry
 */

#ifndef UNIT_H_
#define UNIT_H_

#include <QString>
#include <QVector>
#include <QSharedPointer>
#include <QtSql/QSqlQuery>

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
      static Dimension fromHumanReadable(const QString& str);
      static QString toHumanReadable(Dimension dim);
    };

    static QSharedPointer<const Unit> getPreferredUnit
      (Dimensions::Dimension dimension = Dimensions::PREFERRED_DIMENSION);

    static QSharedPointer<const Unit> getUnit(const QString& abbreviation);

    static QVector<QSharedPointer<const Unit> > getAllUnits();

    static QVector<QSharedPointer<const Unit> > getAllUnits(Dimensions::Dimension dimension);

    virtual ~Unit();

    inline QString getAbbreviation() const { return abbreviation; }

    inline QString getName() const { return name; }

    inline QString getNameAndAbbreviation() const
      { return name + " (" + abbreviation + ")"; }

    inline Dimensions::Dimension getDimension() const { return dimension; }

    // Conversion factor is what you need to multiply an amount in unit by
    // in order to get the amount in terms of otherUnit.
    double getConversionFactor
      (const QSharedPointer<const Unit>& otherUnit = QSharedPointer<const Unit>()) const;

  private:

    Unit(const QString& abbreviation, const QString& name,
          Dimensions::Dimension dimension, double basicConversionFactor);

    QString abbreviation;
    QString name;
    Dimensions::Dimension dimension;
    double basicConversionFactor;

    static QSharedPointer<const Unit> getBasicUnit(Dimensions::Dimension dimension);

    static QVector<QSharedPointer<const Unit> > createUnitsFromQueryResults(QSqlQuery& query);
};

#endif /* UNIT_H_ */
