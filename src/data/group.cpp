/*
 * group.cpp
 * Part of nutrition_tracker
 *
 *  Created on: May 16, 2010
 *      Author: Tyler McHenry <tyler@nerdland.net>
 */

#include "group.h"
#include <QVariant>
#include <QDebug>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlRecord>
#include <QtSql/QSqlField>
#include <QtSql/QSqlError>

const QString Group::DEFAULT_GROUP_ID = "9999";

QSharedPointer<const Group> Group::getDefaultGroup()
{
  return getGroup(DEFAULT_GROUP_ID);
}

QSharedPointer<const Group> Group::getGroup(const QString& id)
{
  QSqlDatabase db = QSqlDatabase::database("nutrition_db");
  QSqlQuery query(db);

  query.prepare("SELECT FdGrp_Cd, FdGrp_Desc FROM group_description WHERE FdGrp_cd=:id "
                "ORDER BY FdGrp_Desc ASC LIMIT 1");
  query.bindValue(":id", id);

  if (query.exec() && query.first()) {
    return createGroupFromRecord(query.record());
  } else {
    return QSharedPointer<const Group>();
  }
}

QVector<QSharedPointer<const Group> > Group::getAllGroups()
{
  QSqlDatabase db = QSqlDatabase::database("nutrition_db");
  QSqlQuery query(db);

  if (query.exec("SELECT FdGrp_Cd, FdGrp_Desc FROM group_description "
                  "ORDER BY FdGrp_Desc ASC")) {
    return createGroupsFromQueryResults(query);
  } else {
    return QVector<QSharedPointer<const Group> >();
  }
}

QSharedPointer<const Group> Group::createGroupFromRecord(const QSqlRecord& record)
{
  if (!record.isEmpty()) {
    return QSharedPointer<const Group>
      (new Group(record.field("FdGrp_Cd").value().toString(),
                 record.field("FdGrp_Desc").value().toString()));
  } else {
    return QSharedPointer<const Group>();
  }
}

QVector<QSharedPointer<const Group> > Group::createGroupsFromQueryResults
  (QSqlQuery& query)
{
  QVector<QSharedPointer<const Group> > groups;

  while (query.next()) {
    groups.push_back(createGroupFromRecord(query.record()));
  }

  return groups;
}

Group::Group(const QString& id, const QString& name)
  : id(id), name(name)
{
  qDebug() << "Created a food group with ID " << id << " named " << name;
}

Group::~Group()
{
}
