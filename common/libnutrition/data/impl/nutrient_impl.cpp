/*
 * nutrient.cpp
 *
 *  Created on: May 14, 2010
 *      Author: tmchenry
 */

#include "nutrient_impl.h"
#include <QVariant>
#include <QDebug>
#include <stdexcept>

NutrientImpl::NutrientImpl
  (const QString& id, const QString& name, Categories::Category category,
   const QSharedPointer<const Unit>& standardUnit, double rdi)
  : id(id), name(name), category(category), standardUnit(standardUnit), rdi(rdi)
{
  qDebug() << "Created nutrient ID " << id << " named " << name << " in category "
           << Categories::toHumanReadable(category) << ". Standard unit is "
           << standardUnit->getNameAndAbbreviation() << " RDI is " << rdi;
}

NutrientImpl::~NutrientImpl()
{
  qDebug() << "Nutrient ID " << id << " was destroyed.";
}

double NutrientImpl::getDefaultCalorieDensity() const
{
  // These are the reference energy densities given by the USDA. Normally, these
  // are given as 9, 4, and 7 kilocalories/gram, but the extra two decimal
  // places were obtained by converting from the more accurate measures of
  // 37, 17, and 29 kilojoules/gram.

  // Most USDA-entered single foods provide calorie density information specific
  // to that food, which is more accurate than these default values, and that
  // information will be used in preference to this when available. In general,
  // these numbers are OVER-estimates compared to the per-food densities.

  // TODO: There are a few other escoteric nutrients that contribute
  // to calories. Add them here.

  static const double FAT_DENSITY = 8.84;
  static const double CARBOHYDRATE_DENSITY = 4.06;
  static const double PROTEIN_DENSITY = 4.06;
  static const double ALCOHOL_DENSITY = 6.93;

  if (name == FAT_NAME) {
    return FAT_DENSITY;
  } else if (name == CARBOHYDRATE_NAME) {
    return CARBOHYDRATE_DENSITY;
  } else if (name == PROTEIN_NAME) {
    return PROTEIN_DENSITY;
  } else if (name == ALCOHOL_NAME) {
    return ALCOHOL_DENSITY;
  } else if (name == CALORIES_NAME) {
    return 1;
  } else {
    return 0;
  }
}

