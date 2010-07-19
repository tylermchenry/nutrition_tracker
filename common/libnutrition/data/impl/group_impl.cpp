/*
 * group.cpp
 * Part of nutrition_tracker
 *
 *  Created on: May 16, 2010
 *      Author: Tyler McHenry <tyler@nerdland.net>
 */

#include "group_impl.h"
#include "libnutrition/proto/data/group.pb.h"
#include <QDebug>

GroupImpl::GroupImpl(const QString& id, const QString& name)
  : id(id), name(name)
{
  qDebug() << "Created a food group with ID " << id << " named " << name;
}

GroupImpl::~GroupImpl()
{
}

GroupData GroupImpl::serialize() const
{
  GroupData gdata;

  gdata.set_id(id.toStdString());
  gdata.set_name(name.toStdString());

  return gdata;
}
