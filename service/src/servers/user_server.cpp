#include "servers/user_server.h"
#include "servers/data_server.h"
#include "libnutrition/data/user.h"
#include <cassert>

namespace UserServer {

  UserLoadResponseObjects loadUsers(const UserLoadRequest& req)
  {
    UserLoadResponseObjects resp_objs;

    if (req.omit()) return resp_objs;

    if (req.all()) {

      resp_objs.addObjects(User::getAllUsers());

    } else {

      for (int i = 0; i < req.requestedids_size(); ++i) {
        resp_objs.addObject(User::getUser(req.requestedids(i)));
      }

      for (int i = 0; i < req.requestedusernames_size(); ++i) {
        resp_objs.addObject
          (User::getUserByUsername(QString::fromStdString(req.requestedusernames(i))));
      }

    }

    return resp_objs;
  }


}
