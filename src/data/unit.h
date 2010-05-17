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
#include <QWeakPointer>
#include <QtSql/QSqlQuery>

class Unit
{
  public:

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
      static Dimension fromHumanReadable(const QString& str);
      static QString toHumanReadable(Dimension dim);
    };

    static QSharedPointer<const Unit> getPreferredUnit
      (Dimensions::Dimension dimension = Dimensions::PREFERRED_DIMENSION);

    static QSharedPointer<const Unit> getUnit(const QString& abbreviation);

    static QVector<QSharedPointer<const Unit> > getAllUnits();

    static QVector<QSharedPointer<const Unit> > getAllUnits(Dimensions::Dimension dimension);

    static QSharedPointer<const Unit> createUnitFromRecord(const QSqlRecord& record);

    static QVector<QSharedPointer<const Unit> > createUnitsFromQueryResults
      (QSqlQuery& query);

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

    inline bool operator==(const Unit& rhs) const { return abbreviation == rhs.abbreviation; }

  private:

    Unit(const QString& abbreviation, const QString& name,
          Dimensions::Dimension dimension, double basicConversionFactor);

    QString abbreviation;
    QString name;
    Dimensions::Dimension dimension;
    double basicConversionFactor;

    static QSharedPointer<const Unit> getBasicUnit(Dimensions::Dimension dimension);

    static QMap<QString, QWeakPointer<const Unit> > unitCache;
};

#endif /* UNIT_H_ */
