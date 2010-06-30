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

QMap<QString, QSharedPointer<const Group> > Group::groupCache;
QMap<QString, QSharedPointer<const Group> > Group::groupCacheByName;

QSharedPointer<const Group> Group::getDefaultGroup()
{
  return getGroup(DEFAULT_GROUP_ID);
}

QSharedPointer<const Group> Group::getGroup(const QString& id)
{
  QSqlDatabase db = QSqlDatabase::database("nutrition_db");
  QSqlQuery query(db);

  if (groupCache.contains(id)) {
    return groupCache[id];
  }

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
  static bool gotAll = false;

  if (gotAll) {
    // TODO: Make this method return a QList so this conversion is unnecessary
    return groupCacheByName.values().toVector();
  }

  QSqlDatabase db = QSqlDatabase::database("nutrition_db");
  QSqlQuery query(db);

  if (query.exec("SELECT FdGrp_Cd, FdGrp_Desc FROM group_description "
                  "ORDER BY FdGrp_Desc ASC")) {
    gotAll = true;
    return createGroupsFromQueryResults(query);
  } else {
    return QVector<QSharedPointer<const Group> >();
  }
}

QSharedPointer<const Group> Group::createGroupFromRecord(const QSqlRecord& record)
{
  if (!record.isEmpty()) {
    QString id = record.field("FdGrp_Cd").value().toString();
    if (!groupCache.contains(id)) {
      QSharedPointer<const Group> group
        (new Group(id, record.field("FdGrp_Desc").value().toString()));
      groupCache[id] = group;
      groupCacheByName[group->getName()] = group;
      return group;
    } else {
      return groupCache[id];
    }
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
