/*
 * amount.h - Data object for a specific amount of something, with a unit
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

#ifndef AMOUNT_H_
#define AMOUNT_H_

#include <QSharedPointer>
#include <stdexcept>
#include "unit.h"

/* Amount is a data object representing the abstract notion of a measurement.
 * It combines a "substance" with a numeric amount and a unit. The "substance"
 * is provided by means of a pointer, and if the pointer is null, the amount is
 * "undefined".
 *
 * This class provides for modification of the measurement either by changing
 * only the numeric amount or by changing both the numeric amount and the unit.
 *
 * Objects of this type support basic arithmetic operations. Addition and
 * subtraction operations against numbers assume that the numbers are in the
 * same unit as the amount. Addition and subtraction operation against other
 * Amount objects support heterogeneous units (the resulting unit will be that
 * of the left-hand side of the operation) but enforce that the two amounts
 * both refer to the same substance object.
 *
 * Undefined amounts (with a NULL substance object) can become defined only by
 * adding to them an Amount of a defined substance, which functions as copy
 * construction. No other operations are permitted on undefined amounts, and
 * attempts will result in exceptions.
 *
 * This is an abstract base that must be inherited from using the curiously-
 * recurring template pattern. Template argument S should be the class of
 * "substance" to which the amount is referring.Template argument SA must be the
 * "substance amount" class, i.e. the class being derived from this base.
 */
template<typename S, typename SA>
class Amount
{
  public:

    virtual ~Amount() {}

    bool isDefined() const { return (substance != NULL); }

    inline QSharedPointer<const S> getSubstance() const
      { return substance; }

    inline QSharedPointer<const Unit> getUnit() const
      { return unit; }

    double getAmount(const QSharedPointer<const Unit>& otherUnit =
                       QSharedPointer<const Unit>()) const;

    void setAmount(double amount, const QSharedPointer<const Unit>& unit =
                     QSharedPointer<const Unit>());

    SA operator+ (const SA& rhs) const;
    SA operator- (const SA& rhs) const;
    SA operator* (double rhs) const;
    SA operator/ (double rhs) const;

    SA& operator+= (const SA& rhs);
    SA& operator-= (const SA& rhs);
    SA& operator*= (double rhs);
    SA& operator/= (double rhs);

  protected:

    Amount(const QSharedPointer<const S>& substance =
              QSharedPointer<const S>(),
            double amount = 0,
            const QSharedPointer<const Unit>& unit =
              QSharedPointer<const Unit>());

    virtual QString getSubstanceName(bool plural = false) const = 0;

  private:

    QSharedPointer<const S> substance;
    double amount;
    QSharedPointer<const Unit> unit;
};

template<typename S, typename SA>
Amount<S, SA>::Amount(const QSharedPointer<const S>& substance,
                         double amount,
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
double Amount<S,SA>::getAmount
  (const QSharedPointer<const Unit>& otherUnit) const
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
void Amount<S, SA>::setAmount
  (double amount, const QSharedPointer<const Unit>& unit)
{
  if (this->substance != NULL) {
    this->amount = std::max(amount, 0.0);
    if (unit != NULL) {
      this->unit = unit;
    }
  } else {
    throw std::logic_error(("Attempted to change the amount of an undefined " +
                             getSubstanceName() + ".").toStdString());
  }
}

template<typename S, typename SA>
SA Amount<S, SA>::operator+ (const SA& rhs) const
{
  SA amt(*dynamic_cast<const SA*>(this));
  return (amt += rhs);
}

template<typename S, typename SA>
SA Amount<S, SA>::operator- (const SA& rhs) const
{
  SA amt(*dynamic_cast<const SA*>(this));
  return (amt -= rhs);
}

template<typename S, typename SA>
SA Amount<S, SA>::operator* (double rhs) const
{
  SA amt(*dynamic_cast<const SA*>(this));
  return (amt *= rhs);
}

template<typename S, typename SA>
SA Amount<S, SA>::operator/ (double rhs) const
{
  SA amt(*dynamic_cast<const SA*>(this));
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
