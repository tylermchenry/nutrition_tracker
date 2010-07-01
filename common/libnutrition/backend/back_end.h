/*
 * backend.h
 *
 *  Created on: Jul 1, 2010
 *      Author: tyler
 */

#ifndef BACK_END_H_
#define BACK_END_H_

#include "libnutrition/data/single_food.h"
#include "libnutrition/data/composite_food.h"
#include "libnutrition/data/template.h"
#include "libnutrition/data/meal.h"
#include "libnutrition/data/user.h"
#include "libnutrition/data/nutrient.h"
#include "libnutrition/data/unit.h"
#include "libnutrition/data/group.h"

#include <QSharedPointer>
#include <QList>
#include <QMap>
#include <QMultiMap>
#include <QPair>

class BackEnd
{
  public:

    inline static void setBackEnd(const QSharedPointer<BackEnd>& backEnd)
      { BackEnd::backEnd = backEnd; }

    static QSharedPointer<BackEnd> getBackEnd() const
      { return backEnd; }

    virtual ~BackEnd() {};

    /** Foods **/

    struct FoodTypes {
      enum FoodType {
        SingleFood,
        CompositeFood,
        Template
      };
    };

    virtual QSharedPointer<Food> loadFood(FoodTypes::FoodType type, int id) = 0;

    virtual QMultiMap<QString, QPair<FoodTypes::FoodType, int> >
      loadFoodNamesForUser(int userId, bool includeExpired) = 0;


    /** Single Foods **/

    virtual QSharedPointer<SingleFood> loadSingleFood(int id) = 0;

    virtual QMultiMap<QString, int> loadSingleFoodNamesForUser(int userId) = 0;

    virtual void storeSingleFood(const QSharedPointer<SingleFood>& food) = 0;


    /** Composite Foods **/

    virtual QSharedPointer<CompositeFood> loadCompositeFood(int id) = 0;

    virtual QMultiMap<QString, int> loadCompositeFoodNamesForUser
      (int userId, bool includeExpired) = 0;

    virtual void storeCompositeFood(const QSharedPointer<CompositeFood>& food) = 0;


    /** Templates **/

    virtual QSharedPointer<Template> loadTemplate(int id) = 0;

    virtual QMultiMap<QString, int> loadTemplateNamesForUser(int userId) = 0;

    virtual void storeTemplate(const QSharedPointer<Template>& templ) = 0;


    /** Meals **/

    virtual QSharedPointer<Meal> loadMeal
      (int userId, const QDate& date, int mealId) = 0;

    virtual QMultiMap<QString, int> loadAllMealNames() = 0;

    virtual QMultiMap<QString, int> loadAllMealNamesForUser
      (int userId, bool includeGenerics) = 0;

    virtual QList<QSharedPointer<Meal> > loadAllMealsForDay
      (int userId, const QDate& date) = 0;

    virtual void storeMeal(const QSharedPointer<Meal>& meal) = 0;


    /** Users **/

    virtual QSharedPointer<User> loadUser(int id) = 0;

    virtual void storeUser(const QSharedPointer<User>& user) = 0;


    /** Nutrients **/

    virtual QSharedPointer<const Nutrient> loadNutrient(int id) = 0;

    virtual QList<QSharedPointer<const Nutrient> > loadAllNutrients(int id) = 0;


    /** Units **/

    virtual QSharedPointer<const Unit> loadUnit(const QString& abbr) = 0;

    virtual QList<QSharedPointer<const Unit> > loadAllUnits(const QString& abbr) = 0;

    virtual QSharedPointer<const SpecializedUnit>
      loadSpecializedUnit(int foodId, int sequence) = 0;

    virtual QList<QSharedPointer<const SpecializedUnit> >
      loadAllSpecializedUnitsForFood(int foodId) = 0;

    virtual QPair<QList<QSharedPointer<const Unit> >,
                   QList<QSharedPointer<const SpecializedUnit> > >
      loadAllUnitsForFood(int foodId) = 0;

    /** Groups **/

    virtual QSharedPointer<const Group> loadGroup(int id) = 0;

    virtual QList<QSharedPointer<const Group> > loadAllGroups(int id) = 0;


    /** Search **/

    struct SearchRequest {

      QString searchTerms;

      QList<QSharedPointer<const Group> > excludeGroups;

      bool searchSingleFoods;
      bool searchComposites;
      bool searchTemplates;

      bool sourceUSDA;
      bool sourceImport;
      bool sourceSelf;
      bool sourceOthers;
      bool includeExpired;

      int maxResults;

      SearchRequest()
      : searchSingleFoods(true), searchComposites(true),
        searchTemplates(false), sourceUSDA(true), sourceImport(true),
        sourceSelf(true), sourceOthers(true), includeExpired(false),
        maxResults(500)
      {}
    };

    struct SearchResult {

      QString displayName;
      FoodTypes::FoodType foodType;
      int id;

      SearchResult(const QString& dn, FoodTypes::FoodType ft, int id)
        : displayName(dn), foodType(ft), id(id)
      {}
    };

    virtual QList<SearchResult> searchFoods(const SearchRequest& request) = 0;

  private:

    static QSharedPointer<BackEnd> backEnd;
};

#endif /* BACK_END_H_ */
