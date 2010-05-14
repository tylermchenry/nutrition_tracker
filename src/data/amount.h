/*
 * amount.h
 *
 *  Created on: May 14, 2010
 *      Author: tmchenry
 */

#ifndef AMOUNT_H_
#define AMOUNT_H_

#include "unit.h"
#include <stdexcept>

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

    inline const S* getSubstance() const { return substance; }

    inline const Unit* getUnit() const { return unit; }

    double getAmount(const Unit* otherUnit = NULL) const;

    SA operator+ (const SA& rhs) const;
    SA operator- (const SA& rhs) const;
    SA operator* (double rhs) const;
    SA operator/ (double rhs) const;

    SA& operator+= (const SA& rhs);
    SA& operator-= (const SA& rhs);
    SA& operator*= (double rhs);
    SA& operator/= (double rhs);

  protected:

    Amount(const S* substance = NULL, double amount = 0,
           const Unit* unit = NULL);

    virtual QString getSubstanceName(bool plural = false) const = 0;

  private:

    const S* substance;
    double amount;
    const Unit* unit;
};

template<typename S, typename SA>
Amount<S, SA>::Amount(const S* substance, double amount, const Unit* unit)
  : substance(substance), amount(std::max(amount, 0.0)), unit(unit)
{
  // Invariant: Either substance and unit are both NULL, or neither is
  if (unit == NULL) {
    substance = NULL;
  }

  // Invariant: If substance is NULL, amount is zero
  if (substance == NULL) {
    amount = 0;
  }
}

template<typename S, typename SA>
double Amount<S,SA>::getAmount(const Unit* otherUnit) const
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
