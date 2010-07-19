/*
 * composite_food.h - Data object for a template to generate other foods
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

#ifndef TEMPLATE_H_
#define TEMPLATE_H_

#include "food_collection.h"

class TemplateData; // forward decl

class Template : virtual public FoodCollection
{
  public:

    // Definitions to make this class cacheable with DataCache
    typedef int cache_key_type;
    typedef Template cache_object_type;
    static const bool cache_strong = false;

    static QSharedPointer<Template> createNewTemplate
      (const QSharedPointer<const Template>& copy =
       QSharedPointer<const Template>());

    static QSharedPointer<Template> getTemplate(int id);

    static QMultiMap<QString, int> getFoodsForUser(int userId);

    virtual ~Template() {};

    virtual int getTemplateId() const = 0;

    virtual FoodData& serialize(FoodData& fdata) const = 0;

    virtual TemplateData serialize() const = 0;

  protected:

    virtual QSharedPointer<Food> getCanonicalSharedPointer() const;

  private:

    static int tempId;

    friend class TemplateImpl;
};

#endif /* TEMPLATE_H_ */
