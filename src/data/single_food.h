/*
 * single_food.h
 *
 *  Created on: May 14, 2010
 *      Author: tmchenry
 */

#ifndef SINGLE_FOOD_H_
#define SINGLE_FOOD_H_

#include "food.h"
#include "unit.h"

class SingleFood: public Food
{
  public:

    struct EntrySources {
      enum EntrySource {
          USDA,
          Import,
          Custom
      };
      static EntrySource fromHumanReadable(const QString& str);
      static QString toHumanReadable(EntrySource dim);
    };

    explicit SingleFood(int id = 0);

    virtual ~SingleFood();

    int getSingleFoodId() const;

    virtual QMap<QString, NutrientAmount> getNutrients() const;

  private:

    int id;
    EntrySources::EntrySource entrySource;
    QMap<QString, NutrientAmount> nutrients;
};

#endif /* SINGLE_FOOD_H_ */
