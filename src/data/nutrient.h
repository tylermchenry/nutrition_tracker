/*
 * nutrient.h
 *
 *  Created on: May 14, 2010
 *      Author: tmchenry
 */

#ifndef NUTRIENT_H_
#define NUTRIENT_H_

#include <QString>
#include <QSharedPointer>
#include "unit.h"

class Nutrient
{
  public:

    explicit Nutrient(const QString& id = "");
    virtual ~Nutrient();

    inline QString getId() const { return id; }

    inline QString getName() const { return name; }

    inline QSharedPointer<const Unit> getStandardUnit() const { return standardUnit; }

    inline double getRDI() const { return rdi; }

  private:

    QString id;
    QString name;
    QSharedPointer<const Unit> standardUnit;
    double rdi;
};

#endif /* NUTRIENT_H_ */
