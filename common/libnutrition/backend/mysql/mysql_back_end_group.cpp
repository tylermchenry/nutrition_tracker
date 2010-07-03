#include "mysql_back_end.h"
#include "libnutrition/data/data_cache.h"
#include "libnutrition/data/impl/group_impl.h"
#include <QVariant>
#include <QDebug>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlRecord>
#include <QtSql/QSqlField>
#include <QtSql/QSqlError>
#include <stdexcept>

QSharedPointer<const Group> MySQLBackEnd::loadGroup(const QString& id)
{
  QSqlQuery query(db);

  query.prepare("SELECT FdGrp_Cd, FdGrp_Desc FROM group_description WHERE FdGrp_cd=:id "
                "ORDER BY FdGrp_Desc ASC LIMIT 1");
  query.bindValue(":id", id);

  if (query.exec() && query.first()) {
    return createGroupFromRecord(query.record());
  } else {
    return QSharedPointer<Group>();
  }
}

QList<QSharedPointer<Group> > MySQLBackEnd::loadAllGroups()
{
  QSqlQuery query(db);

  if (query.exec("SELECT FdGrp_Cd, FdGrp_Desc FROM group_description "
                 "ORDER BY FdGrp_Desc ASC")) {
    return createGroupsFromQueryResults(query);
  } else {
    return QList<QSharedPointer<Group> >();
  }
}

QList<QSharedPointer<Group> > MySQLBackEnd::createGroupsFromQueryResults
  (QSqlQuery& query)
{
  QList<QSharedPointer<Group> > groups;

  while (query.next()) {
    groups.push_back(createGroupFromRecord(query.record()));
  }

  return groups;
}

QSharedPointer<Group> MySQLBackEnd::createGroupFromRecord
  (const QSqlRecord& record)
{
  if (!record.isEmpty()) {
    QString id = record.field("FdGrp_Cd").value().toString();
    QSharedPointer<Group> group
      (new GroupImpl(id, record.field("FdGrp_Desc").value().toString()));
    DataCache<Group>::getInstance().insert(id, group);
    return group;
  } else {
    return QSharedPointer<Group>();
  }
}
