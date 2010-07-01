/*
 * group.cpp
 * Part of nutrition_tracker
 *
 *  Created on: May 16, 2010
 *      Author: Tyler McHenry <tyler@nerdland.net>
 */

#include "group_impl.h"
#include <QDebug>

GroupImpl::GroupImpl(const QString& id, const QString& name)
  : id(id), name(name)
{
  qDebug() << "Created a food group with ID " << id << " named " << name;
}

GroupImpl::~GroupImpl()
{
}
