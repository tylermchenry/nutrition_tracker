/*
 * service_back_end_group.cpp
 * Part of libnutrition
 *
 *  Created on: Jul 4, 2010
 *      Author: Tyler McHenry <tyler@nerdland.net>
 */

#include "service_back_end.h"
#include "libnutrition/proto/service/group_messages.pb.h"
#include "libnutrition/data/impl/group_impl.h"
#include "libnutrition/data/data_cache.h"
#include <stdexcept>

QSharedPointer<const Group> ServiceBackEnd::loadGroup(const QString& id)
{
  GroupLoadRequest req;
  GroupLoadResponse resp;
  LoadedData ldata;

  req.add_requestedids(id.toStdString());

  writeMessageAndReadResponse(req, resp);
  loadResponseData(ldata, resp);

  return ldata.groups[id];
}

QList<QSharedPointer<Group> > ServiceBackEnd::loadAllGroups()
{
  GroupLoadRequest req;
  GroupLoadResponse resp;
  LoadedData ldata;

  req.set_all(true);

  writeMessageAndReadResponse(req, resp);
  loadResponseData(ldata, resp);

  QList<QSharedPointer<Group> > all = ldata.groups.values();

  // We will instruct server to omit groups on future data requests
  loadedAllGroups = true;

  return all;
}

void ServiceBackEnd::loadResponseData
  (LoadedData& loadedData, const GroupLoadResponse& resp)
{
  if (resp.has_error() && resp.error().iserror()) {
    throw std::runtime_error("Group load error: " + resp.error().errormessage());
  }

  for (int i = 0; i < resp.groups_size(); ++i) {

    const GroupData& gdata = resp.groups(i);
    QString id = QString::fromStdString(gdata.id());

    QSharedPointer<Group> group = QSharedPointer<Group>
      (new GroupImpl(id, QString::fromStdString(gdata.name())));

    DataCache<Group>::getInstance().insert(id, group);
    loadedData.groups.insert(id, group);
  }
}
