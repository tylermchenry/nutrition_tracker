/*
 * service_back_end_user.cpp
 * Part of libnutrition
 *
 *  Created on: Jul 4, 2010
 *      Author: Tyler McHenry <tyler@nerdland.net>
 */

/*
 * service_back_end_user.cpp
 * Part of libnutrition
 *
 *  Created on: Jul 4, 2010
 *      Author: Tyler McHenry <tyler@nerdland.net>
 */

#include "service_back_end.h"
#include "libnutrition/proto/data.pb.h"
#include "libnutrition/data/impl/user_impl.h"
#include "libnutrition/data/data_cache.h"
#include <stdexcept>

QSharedPointer<User> ServiceBackEnd::loadUser(int id)
{
  DataLoadRequest req;
  DataLoadResponse resp;
  LoadedData ldata;

  req.mutable_userloadrequest()->add_requestedids(id);
  setOmissions(req);

  DataCache<User>::getInstance().remove(id);

  writeMessageAndReadResponse(req, resp);
  loadResponseData(ldata, resp);

  return ldata.users[id];
}

QSharedPointer<User> ServiceBackEnd::loadUserByUsername(const QString& username)
{
  DataLoadRequest req;
  DataLoadResponse resp;
  LoadedData ldata;

  req.mutable_userloadrequest()->add_requestedusernames(username.toStdString());
  setOmissions(req);

  // TODO: How to remove user from cache by username?

  writeMessageAndReadResponse(req, resp);
  loadResponseData(ldata, resp);

  for (QMap<int, QSharedPointer<User> >::const_iterator i = ldata.users.begin();
      i != ldata.users.end(); ++i)
  {
    if (i.value()->getUsername() == username) {
      return i.value();
    }
  }

  return QSharedPointer<User>();
}

QList<QSharedPointer<User> > ServiceBackEnd::loadAllUsers()
{
  DataLoadRequest req;
  DataLoadResponse resp;
  LoadedData ldata;

  req.mutable_userloadrequest()->set_all(true);
  setOmissions(req);

  DataCache<User>::getInstance().clear();

  writeMessageAndReadResponse(req, resp);
  loadResponseData(ldata, resp);

  return ldata.users.values();
}

void ServiceBackEnd::loadResponseData
  (LoadedData& loadedData, const UserLoadResponse& resp)
{
  if (resp.has_error() && resp.error().iserror()) {
    throw std::runtime_error("User load error: " + resp.error().errormessage());
  }

  for (int i = 0; i < resp.users_size(); ++i) {

    const UserData& udata = resp.users(i);
    int id = udata.userid();

    QSharedPointer<User> user = DataCache<User>::getInstance().get(id);

    if (!user) {
      user = QSharedPointer<User>
        (new UserImpl
            (id,
             QString::fromStdString(udata.username()),
             QString::fromStdString(udata.realname()),
             QString::fromStdString(udata.password_sha1_hex())));
    }

    DataCache<User>::getInstance().insert(id, user);
    loadedData.users.insert(id, user);
  }
}
