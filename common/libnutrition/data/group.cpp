/*
 * group.cpp
 * Part of nutrition_tracker
 *
 *  Created on: May 16, 2010
 *      Author: Tyler McHenry <tyler@nerdland.net>
 */

#include "group.h"
#include "impl/group_impl.h"
#include "data_cache.h"
#include <QVariant>
#include <QDebug>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlRecord>
#include <QtSql/QSqlField>
#include <QtSql/QSqlError>

const QString Group::DEFAULT_GROUP_ID = "9999";

QString (Group::* const Group::cache_get_sort_key)() const = &Group::getName;

QSharedPointer<const Group> Group::getDefaultGroup()
{
  return getGroup(DEFAULT_GROUP_ID);
}

QSharedPointer<const Group> Group::getGroup(const QString& id)
{
  QSqlDatabase db = QSqlDatabase::database("nutrition_db");
  QSqlQuery query(db);

  if (DataCache<Group>::getInstance().contains(id)) {
    return DataCache<Group>::getInstance().get(id);
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
    static QVector<QSharedPointer<const Group> > groupsByName;

    if (groupsByName.empty()) {
      groupsByName = DataCache<Group>::getInstance().getAll().toVector();
    }

    return groupsByName;
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
    if (!DataCache<Group>::getInstance().contains(id)) {
      QSharedPointer<const Group> group
        (new GroupImpl(id, record.field("FdGrp_Desc").value().toString()));
      DataCache<Group>::getInstance().insert(id, group);
      return group;
    } else {
      return DataCache<Group>::getInstance().get(id);
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
