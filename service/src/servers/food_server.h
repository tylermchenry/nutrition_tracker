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
#include "libnutrition/proto/service/data_messages.pb.h"
#include "servers/response_objects.h"
#include <QString>
#include <QSet>
#include <stdexcept>

class SingleFoodLoadResponse; // forward decl
class CompositeFoodLoadResponse; // forward decl
class TemplateLoadResponse; // forward decl

class FoodLoadResponseObjects : public ResponseObjects<Food, DataLoadResponse, QString>
{
  public:

    void setError(BackEnd::FoodTypes::FoodType foodType, const QString& errorMessage = "");

    DataLoadResponse& serialize(DataLoadResponse& resp, const Omissions& omissions) const;

    SingleFoodLoadResponse serializeSingleFoods() const;

    CompositeFoodLoadResponse serializeCompositeFoods() const;

    TemplateLoadResponse serializeTemplates() const;

  protected:

    virtual bool isValid
      (const QSharedPointer<const Food>& food) const
        { return !food.isNull(); }

    virtual QString getId
      (const QSharedPointer<const Food>& food) const
        { return food->getId(); }

    virtual void addObjectToResponse
      (DataLoadResponse&, const QSharedPointer<const Food>&) const
        { throw std::logic_error("Called wrong serialization method on FoodResponseObjects."); }

  private:

    struct Error {
      bool isError;
      QString errorMessage;
      Error() : isError(false) {};
    };

    QMap<BackEnd::FoodTypes::FoodType, Error> subErrors;

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
