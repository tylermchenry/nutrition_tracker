#include "servers/group_server.h"
#include "servers/data_server.h"
#include "libnutrition/data/group.h"
#include <cassert>

namespace GroupServer {

  GroupLoadResponseObjects loadGroups(const GroupLoadRequest& req)
  {
    GroupLoadResponseObjects resp_objs;

    if (req.omit()) return resp_objs;

    if (req.all()) {

      resp_objs.addObjects(Group::getAllGroups());

    } else {

      for (int i = 0; i < req.requestedids_size(); ++i) {
        resp_objs.addObject
          (Group::getGroup(QString::fromStdString(req.requestedids(i))));
      }

    }

    return resp_objs;
  }
}
