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

class Template : public FoodCollection
{
  public:

    static QSharedPointer<Template> createNewTemplate
      (const QSharedPointer<const Template>& copy =
       QSharedPointer<const Template>());

    static QSharedPointer<Template> getTemplate(int id);

    static QSharedPointer<Template>
      createTemplateFromQueryResults(QSqlQuery& query);

    static QMultiMap<QString, int> getFoodsForUser(int userId);

    virtual ~Template();

    inline int getTemplateId() const { return id; }

    virtual void saveToDatabase();

    virtual void deleteFromDatabase();

  protected:

    virtual QSharedPointer<Food> getCanonicalSharedPointer();

    virtual QSharedPointer<const Food> getCanonicalSharedPointer() const;

  private:

    Template(int id, const QString& name,
              const QList<FoodComponent>& components = QList<FoodComponent>());

    // Default or "Copy" constructor. If a food is passed in to copy, the
    // attributes of the food object are copied, but the constructed food
    // is still assigned a new, temporary ID.
    Template(const QSharedPointer<const Template>& copy =
              QSharedPointer<const Template>());

    int id;

    static int tempId;

    static QMap<int, QWeakPointer<Template> > templateCache;

};

#endif /* TEMPLATE_H_ */
