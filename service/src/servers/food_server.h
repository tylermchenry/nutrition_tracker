/*
 * food_server.h
 *
 *  Created on: Jul 21, 2010
 *      Author: tyler
 */

#ifndef FOOD_SERVER_H_
#define FOOD_SERVER_H_

#include "servers/omissions.h"
#include "libnutrition/data/food.h"
#include "libnutrition/backend/back_end.h"
#include <QString>
#include <QSet>

class DataLoadResponse; // forward decl
class SingleFoodLoadResponse; // forward decl
class CompositeFoodLoadResponse; // forward decl
class TemplateLoadResponse; // forward decl

class FoodLoadResponseObjects
{
  public:

    void addFood(const QSharedPointer<const Food>& food);
    void addFoods(const QVector<QSharedPointer<const Food> >& foods);
    void addFoods(const QList<QSharedPointer<const Food> >& foods);

    void clear();

    void replaceFoods(const QList<QSharedPointer<const Food> >& foods);

    void setError(BackEnd::FoodTypes::FoodType foodType, const QString& errorMessage = "");
    void setError(const QString& errorMessage = "");

    inline bool isEmpty() const { return foods.isEmpty(); }
    bool contains(const QSharedPointer<const Food>& food) const;

    QList<QSharedPointer<const Food> > getFoods() const;
    QSet<QString> getFoodIds() const;

    DataLoadResponse& serialize(DataLoadResponse& resp, const Omissions& omissions) const;

    SingleFoodLoadResponse serializeSingleFoods() const;

    CompositeFoodLoadResponse serializeCompositeFoods() const;

    TemplateLoadResponse serializeTemplates() const;

  private:

    struct Error {
      bool isError;
      QString errorMessage;
      Error() : isError(false) {};
    };

    Error error;
    QMap<BackEnd::FoodTypes::FoodType, Error> subErrors;

    QSet<QString> foodIds;
    QList<QSharedPointer<const Food> > foods;

    FoodData getFoodData() const;

    SingleFoodLoadResponse& serializeSingleFoods
      (const FoodData& fdata, SingleFoodLoadResponse& resp) const;

    CompositeFoodLoadResponse& serializeCompositeFoods
      (const FoodData& fdata, CompositeFoodLoadResponse& resp) const;

    TemplateLoadResponse& serializeTemplates
      (const FoodData& fdata, TemplateLoadResponse& resp) const;

    template <typename T>
    static void setError(T& resp, const Error& err);
};


#endif /* FOOD_SERVER_H_ */
