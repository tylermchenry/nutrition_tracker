#ifndef UNIT_SERVER_H_
#define UNIT_SERVER_H_

#include "libnutrition/proto/service/unit_messages.pb.h"
#include "libnutrition/data/unit.h"
#include <QString>
#include <QSet>

class UnitLoadResponseObjects
{
  public:

    void addUnit(const QSharedPointer<const Unit>& unit);
    void addUnits(const QVector<QSharedPointer<const Unit> >& units);
    void addUnits(const QList<QSharedPointer<const Unit> >& units);

    inline bool isEmpty() const { return units.isEmpty(); }

    QList<QSharedPointer<const Unit> > getUnits() const;

    UnitLoadResponse serialize() const;

  private:

    QSet<QString> unitIds;
    QList<QSharedPointer<const Unit> > units;
};

namespace UnitServer
{
  UnitLoadResponseObjects loadUnits(const UnitLoadRequest& req);
}

#endif /* UNIT_SERVER_H_ */
