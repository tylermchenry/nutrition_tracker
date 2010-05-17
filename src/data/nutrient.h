/*
 * nutrient.h
 *
 *  Created on: May 14, 2010
 *      Author: tmchenry
 */

#ifndef NUTRIENT_H_
#define NUTRIENT_H_

#include <QString>
#include <QMap>
#include <QSharedPointer>
#include <QWeakPointer>
#include <QtSql/QSqlQuery>
#include "data/unit.h"

class Nutrient
{
  public:

    struct Categories {
      enum Category {
          Energy,
          Basic,
          Vitamin,
          Mineral,
          Additional,
          Hidden
      };
      static Category fromHumanReadable(const QString& str);
      static QString toHumanReadable(Category cat);
    };

    static QSharedPointer<const Nutrient> getNutrient(const QString& id);

    static QVector<QSharedPointer<const Nutrient> > getAllNutrients();

    static QVector<QSharedPointer<const Nutrient> > getAllNutrients
      (Categories::Category category);

    static QSharedPointer<const Nutrient> createNutrientFromRecord(const QSqlRecord& record);

    static QVector<QSharedPointer<const Nutrient> > createNutrientsFromQueryResults
      (QSqlQuery& query);

    virtual ~Nutrient();

    inline QString getId() const { return id; }

    inline QString getName() const { return name; }

    inline Categories::Category getCategory() const { return category; }

    inline QSharedPointer<const Unit> getStandardUnit() const { return standardUnit; }

    inline double getRDI() const { return rdi; }

  private:

    Nutrient(const QString& id, const QString& name, Categories::Category category,
              const QSharedPointer<const Unit>& standardUnit, double rdi);

    QString id;
    QString name;
    Categories::Category category;
    QSharedPointer<const Unit> standardUnit;
    double rdi; // RDI in terms of standard unit

    static QMap<QString, QWeakPointer<const Nutrient> > nutrientCache;
};

#endif /* NUTRIENT_H_ */
