/*
 * group.h
 * Part of nutrition_tracker
 *
 *  Created on: May 16, 2010
 *      Author: Tyler McHenry <tyler@nerdland.net>
 */

#ifndef GROUP_H_
#define GROUP_H_

#include <QVector>
#include <QMap>
#include <QSharedPointer>
#include <QWeakPointer>
#include <QtSql/QSqlQuery>

class Group
{
  public:

    static const QString DEFAULT_GROUP_ID;

    static QSharedPointer<const Group> getDefaultGroup();

    static QSharedPointer<const Group> getGroup(const QString& id);

    static QVector<QSharedPointer<const Group> > getAllGroups();

    static QSharedPointer<const Group> createGroupFromRecord
      (const QSqlRecord& record);

    static QVector<QSharedPointer<const Group> > createGroupsFromQueryResults
      (QSqlQuery& query);

    virtual ~Group();

    inline QString getId() const { return id; }

    inline QString getName() const { return name; }

    inline bool operator== (const Group& rhs) const { return (id == rhs.id); }

  private:

    Group(const QString& id, const QString& name);

    QString id;
    QString name;

    static QMap<QString, QWeakPointer<const Group> > groupCache;
};

#endif /* GROUP_H_ */
