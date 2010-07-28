/*
 * single_food.cpp
 *
 *  Created on: May 14, 2010
 *      Author: tmchenry
 */

#include "single_food_impl.h"
#include "libnutrition/proto/data/data.pb.h"
#include "libnutrition/backend/back_end.h"
#include <QDebug>

SingleFoodImpl::SingleFoodImpl
  (int id, const QString& name, int ownerId,
   EntrySources::EntrySource entrySource,
   const QSharedPointer<const Group>& group,
   double percentRefuse, const QString& refuseDescription,
   const QMap<QString, NutrientAmount>& nutrients,
   double weightAmount, double volumeAmount,
   double quantityAmount, double servingAmount,
   double calorieDensityFat, double calorieDensityCarbohydrate,
   double calorieDensityProtien, double calorieDensityAlcohol)

  : FoodImpl("SINGLE_" + QString::number(id), name, ownerId,
             weightAmount, volumeAmount, quantityAmount, servingAmount),
    id(id), entrySource(entrySource), group(group),
    percentRefuse(percentRefuse), refuseDescription(refuseDescription),
    nutrients(nutrients)
{
  qDebug() << "Created food ID " << id << " named " << name << " from "
           << EntrySources::toHumanReadable(entrySource)
           << " in group: " << group->getName()
           << " refuse: " << percentRefuse << "% "
           << refuseDescription << " amounts: "
           << weightAmount << " g, " << volumeAmount << " fl oz "
           << quantityAmount << "qty, " << servingAmount << " srv."
           << " densities: fat =" << calorieDensityFat
           << " carb = " << calorieDensityCarbohydrate
           << " protein = " << calorieDensityProtien
           << " alcohol = " << calorieDensityAlcohol;

#ifdef VERBOSE_DEBUG
  qDebug() << "Nutrient Amounts:";

  for (QMap<QString, NutrientAmount>::const_iterator i = nutrients.begin();
       i != nutrients.end(); ++i)
  {
    qDebug() << i.value().getAmount() << " " << i.value().getUnit()->getNameAndAbbreviation()
             << " (" << i.value().getAmountAsPercentRDI() * 100 << "% RDI) of "
             << i.value().getNutrient()->getName();
  }
#endif

  setCalorieDensity(Nutrient::FAT_NAME, calorieDensityFat);
  setCalorieDensity(Nutrient::CARBOHYDRATE_NAME, calorieDensityCarbohydrate);
  setCalorieDensity(Nutrient::PROTEIN_NAME, calorieDensityProtien);
  setCalorieDensity(Nutrient::ALCOHOL_NAME, calorieDensityAlcohol);
  sanityCheckCalorieDensities();
}

SingleFoodImpl::SingleFoodImpl(const QSharedPointer<const SingleFood>& copy)
  : FoodImpl("SINGLE_" + QString::number(tempId), copy),
    id(tempId--), entrySource(copy ? copy->getEntrySource() : EntrySources::Custom),
    group(copy ? copy->getGroup() : Group::getDefaultGroup())
{
  qDebug() << "Created new food with temporary ID " << id;

  if (copy) {
    calorieDensities = copy->getCalorieDensities();
  } else {
    setCalorieDensity(Nutrient::FAT_NAME, 0);
    setCalorieDensity(Nutrient::CARBOHYDRATE_NAME, 0);
    setCalorieDensity(Nutrient::PROTEIN_NAME, 0);
    setCalorieDensity(Nutrient::ALCOHOL_NAME, 0);
  }
}

SingleFoodImpl::~SingleFoodImpl()
{
}

QMap<QString, NutrientAmount> SingleFoodImpl::getNutrients() const
{
  return nutrients;
}

NutrientAmount SingleFoodImpl::getCaloriesFromNutrientId
  (const QString& nutrId) const
{
  QSharedPointer<const Nutrient> calories =
    Nutrient::getNutrientByName(Nutrient::CALORIES_NAME);

  if (nutrients.contains(nutrId) && calorieDensities.contains(nutrId)) {
    return NutrientAmount(calories,
                          nutrients[nutrId].getAmount(Unit::getUnit("g")) *
                          calorieDensities[nutrId]);
  } else if (nutrId == calories->getId() && nutrients.contains(nutrId)) {
    return NutrientAmount(calories, nutrients[nutrId].getAmount());
  } else {
    return NutrientAmount(calories, 0);
  }
}

double SingleFoodImpl::getCalorieDensityForNutrientId
  (const QString& nutrId) const
{
  if (calorieDensities.contains(nutrId)) {
    return calorieDensities[nutrId];
  } else {
    return 0;
  }
}

void SingleFoodImpl::setEntrySource(EntrySources::EntrySource source)
{
  this->entrySource = source;
}

void SingleFoodImpl::setGroup(const QSharedPointer<const Group>& group)
{
  if (group != NULL) {
    this->group = group;
  }
}

void SingleFoodImpl::setNutrient(const NutrientAmount& nutrientAmount)
{
  if (nutrientAmount.isDefined()) {

    // It's important to check that we're actually changing the value of the nutrient amount,
    // rather than overwriting it with itself. The reason is that nutrient amounts for foods
    // from the USDA source contain a lot of extra information about sample sizes, standard
    // deviations, etc, that won't be valid anymore if the value is changed. We need to
    // make sure that we only update the rows in the DB for which the value has actually
    // changed, in order to handle this properly.

    QString id = nutrientAmount.getNutrient()->getId();
    if (!nutrients[id].isDefined() ||
        (nutrients[id].getAmount() != nutrientAmount.getAmount(nutrients[id].getUnit())))
    {
      modifiedNutrients.insert(id);
      nutrients[id] = nutrientAmount;
      sanityCheckCalorieDensities();
    }
  }
}

void SingleFoodImpl::saveToDatabase()
{
  BackEnd::getBackEnd()->storeSingleFood
    (getCanonicalSharedPointer().dynamicCast<SingleFood>());
}

void SingleFoodImpl::deleteFromDatabase()
{
  BackEnd::getBackEnd()->deleteSingleFood
    (getCanonicalSharedPointer().dynamicCast<SingleFood>());
}

FoodData& SingleFoodImpl::serialize(FoodData& fdata) const
{
  *(fdata.add_singlefoods()) = serialize();
  return fdata;
}

SingleFoodData SingleFoodImpl::serialize() const
{
  SingleFoodData sfdata;

  sfdata.set_id(id);
  sfdata.set_name(getName().toStdString());
  sfdata.set_ownerid(getOwnerId());

  sfdata.set_entrysource(static_cast<SingleFoodData::EntrySource>(entrySource));
  sfdata.set_groupid(group->getId().toStdString());
  sfdata.set_percentrefuse(percentRefuse);
  sfdata.set_refusedescription(refuseDescription.toStdString());

  for (QMap<QString, NutrientAmount>::const_iterator i = nutrients.begin();
       i != nutrients.end(); ++i)
  {
     SingleFoodData_NutrientAmount* namt = sfdata.add_nutrientamounts();
     namt->set_nutrientid(i.value().getNutrient()->getId().toStdString());
     namt->set_amount(i.value().getAmount());
     namt->set_unitabbreviation(i.value().getUnit()->getAbbreviation().toStdString());
  }

  sfdata.set_weightamount(getBaseAmount(Unit::Dimensions::Weight).getAmount());
  sfdata.set_volumeamount(getBaseAmount(Unit::Dimensions::Volume).getAmount());
  sfdata.set_quantityamount(getBaseAmount(Unit::Dimensions::Quantity).getAmount());
  sfdata.set_servingamount(getBaseAmount(Unit::Dimensions::Serving).getAmount());

  sfdata.set_energydensityfat(calorieDensities[Nutrient::FAT_NAME]);
  sfdata.set_energydensityprotein(calorieDensities[Nutrient::PROTEIN_NAME]);
  sfdata.set_energydensitycarbohydrate(calorieDensities[Nutrient::CARBOHYDRATE_NAME]);
  sfdata.set_energydensityalcohol(calorieDensities[Nutrient::ALCOHOL_NAME]);

  return sfdata;
}

void SingleFoodImpl::setCalorieDensity(const QString& nutrientName, double density)
{
  QSharedPointer<const Nutrient> nutrient = Nutrient::getNutrientByName(nutrientName);
  if (nutrient) {
    calorieDensities[nutrient->getId()] = (density > 0) ? density :
      nutrient->getDefaultCalorieDensity();
  }
}

void SingleFoodImpl::sanityCheckCalorieDensities()
{
  QString caloriesId = Nutrient::getNutrientByName(Nutrient::CALORIES_NAME)->getId();

  if (nutrients.contains(caloriesId)) {

    NutrientAmount calories = nutrients[caloriesId];

    if (calories.getAmount() <= 0) return;

    NutrientAmount computedCalories = NutrientAmount(Nutrient::getNutrient(caloriesId), 0);

    NutrientAmount caloriesFromFat = getCaloriesFromNutrientName(Nutrient::FAT_NAME);
    NutrientAmount caloriesFromCarbs = getCaloriesFromNutrientName(Nutrient::CARBOHYDRATE_NAME);
    NutrientAmount caloriesFromProtein = getCaloriesFromNutrientName(Nutrient::PROTEIN_NAME);
    NutrientAmount caloriesFromAlcohol = getCaloriesFromNutrientName(Nutrient::ALCOHOL_NAME);

    if (caloriesFromFat.isDefined()) computedCalories += caloriesFromFat;
    if (caloriesFromCarbs.isDefined()) computedCalories += caloriesFromCarbs;
    if (caloriesFromProtein.isDefined()) computedCalories += caloriesFromProtein;
    if (caloriesFromAlcohol.isDefined()) computedCalories += caloriesFromAlcohol;

    // It's OK if the computed calories are smaller than the nominal calories;
    // that just means that some calories came from sources other than fat,
    // carbs, protein, and alcohol. But it's not OK if the computed calories are
    // larger than the nominal calories, since that means we are overestimating
    // one or more of the energy densities, which will screw up calories-from-
    // nutrient calculations elsewhere.

    // Since there's no way to determine exactly which densities are wrong and
    // by how much, we simply perform a uniform scaling to reconcile the
    // computed calories with the nominal calories.

    // This will normally happen with user-entered foods which are using the
    // USDA default calorie densities, which appear to be upper bounds. Foods
    // sourced from the USDA database have food-specific calorie densities that
    // are more accurate.

    double scaleFactor = calories.getAmount() / computedCalories.getAmount();

    if (scaleFactor < 1) {
      for (QMap<QString, double>::iterator i = calorieDensities.begin();
          i != calorieDensities.end(); ++i)
      {
        i.value() *= scaleFactor;
      }
    }
  }
}
