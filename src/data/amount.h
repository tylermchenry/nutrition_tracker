/*
 * amount.h
 *
 *  Created on: May 14, 2010
 *      Author: tmchenry
 */

#ifndef AMOUNT_H_
#define AMOUNT_H_

#include <QSharedPointer>
#include <stdexcept>
#include "unit.h"

/*
 * This is an abstract base that must be inherited from using the
 * curiously-recurring template pattern. Template argument S should
 * be the class of "substance" to which the amount is referring.
 * Template argument SA must be the "substance amount" class, i.e.
 * the class being derived from this base.
 */
template<typename S, typename SA>
class Amount
{
  public:

    virtual ~Amount() {}

    inline QSharedPointer<const S> getSubstance() const { return substance; }

    inline QSharedPointer<const Unit> getUnit() const { return unit; }

    double getAmount(const QSharedPointer<const Unit>& otherUnit = QSharedPointer<const Unit>()) const;

    SA operator+ (const SA& rhs) const;
    SA operator- (const SA& rhs) const;
    SA operator* (double rhs) const;
    SA operator/ (double rhs) const;

    SA& operator+= (const SA& rhs);
    SA& operator-= (const SA& rhs);
    SA& operator*= (double rhs);
    SA& operator/= (double rhs);

  protected:

    Amount(const QSharedPointer<const S>& substance = QSharedPointer<const S>(),
           double amount = 0,
           const QSharedPointer<const Unit>& unit = QSharedPointer<const Unit>());

    virtual QString getSubstanceName(bool plural = false) const = 0;

  private:

    QSharedPointer<const S> substance;
    double amount;
    QSharedPointer<const Unit> unit;
};

template<typename S, typename SA>
Amount<S, SA>::Amount(const QSharedPointer<const S>& substance, double amount,
                      const QSharedPointer<const Unit>& unit)
  : substance(substance), amount(std::max(amount, 0.0)), unit(unit)
{
  // Invariant: Either substance and unit are both NULL, or neither is
  if (unit == NULL || substance == NULL) {
    this->substance = QSharedPointer<const S>();
    this->unit = QSharedPointer<const Unit>();
  }

  // Invariant: If substance is NULL, amount is zero
  if (this->substance == NULL) {
    this->amount = 0;
  }
}

template<typename S, typename SA>
double Amount<S,SA>::getAmount(const QSharedPointer<const Unit>& otherUnit) const
{
  if (substance == NULL) {
    return 0;
  } else if (otherUnit == NULL) {
    return amount;
  } else {
    return std::max(amount * unit->getConversionFactor(otherUnit), 0.0);
  }
}

template<typename S, typename SA>
SA Amount<S, SA>::operator+ (const SA& rhs) const
{
  SA amt(*dynamic_cast<SA*>(this));
  return (amt += rhs);
}

template<typename S, typename SA>
SA Amount<S, SA>::operator- (const SA& rhs) const
{
  SA amt(*dynamic_cast<SA*>(this));
  return (amt -= rhs);
}

template<typename S, typename SA>
SA Amount<S, SA>::operator* (double rhs) const
{
  SA amt(*dynamic_cast<SA*>(this));
  return (amt *= rhs);
}

template<typename S, typename SA>
SA Amount<S, SA>::operator/ (double rhs) const
{
  SA amt(*dynamic_cast<SA*>(this));
  return (amt /= rhs);
}

template<typename S, typename SA>
SA& Amount<S, SA>::operator+= (const SA& rhs)
{
  if (rhs.substance == NULL) {
    throw std::logic_error(("Attempted to add an undefined " +
                             getSubstanceName() + ".").toStdString());
  }

  // Undefined substances can be defined by adding a defined substance to them
  if (substance == NULL) {
    substance = rhs.substance;
    unit = rhs.unit;
  }

  if (substance->getId() == rhs.substance->getId()) {
    amount = std::max(amount + rhs.getAmount(unit), 0.0);
  } else {
    throw std::logic_error(("Attempted to add amounts of two different " +
                            getSubstanceName(true) + ".").toStdString());
  }

  return dynamic_cast<SA&>(*this);
}

template<typename S, typename SA>
SA& Amount<S, SA>::operator-= (const SA& rhs)
{
  if (rhs.substance == NULL) {
    throw std::logic_error(("Attempted to subtract an undefined " +
                            getSubstanceName() + ".").toStdString());
  }

  if (substance == NULL) {
    throw std::logic_error(("Attempted to subtract from an undefined " +
                            getSubstanceName() + ".").toStdString());
  }

  if (substance->getId() == rhs.substance->getId()) {
    amount = std::max(amount - rhs.getAmount(unit), 0.0);
  } else {
    throw std::logic_error(("Attempted to subtract amounts of two different " +
                            getSubstanceName(true) + ".").toStdString());
  }

  return dynamic_cast<SA&>(*this);
}

template<typename S, typename SA>
SA& Amount<S, SA>::operator*= (double rhs)
{
  if (substance == NULL) {
    throw std::logic_error(("Attempted to multiply an undefined " +
                            getSubstanceName() + ".").toStdString());
  }

  amount = std::max(amount * rhs, 0.0);

  return dynamic_cast<SA&>(*this);
}

template<typename S, typename SA>
SA& Amount<S, SA>::operator/= (double rhs)
{
  if (substance == NULL) {
    throw std::logic_error(("Attempted to divide an undefined " +
                            getSubstanceName() + ".").toStdString());
  } else if (rhs == 0) {
    throw std::logic_error(("Attempted to divide a " +
                             getSubstanceName() + " by zero.").toStdString());
  }

  amount = std::max(amount / rhs, 0.0);

  return dynamic_cast<SA&>(*this);
}

#endif /* AMOUNT_H_ */
