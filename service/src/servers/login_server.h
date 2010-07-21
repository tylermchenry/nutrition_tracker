#ifndef LOGIN_SERVER_H_
#define LOGIN_SERVER_H_

#include "libnutrition/proto/service/login_messages.pb.h"

namespace LoginServer {

  LogInResponse doLogin(const LogInRequest& req, int& loggedInUserId);

}

#endif /* LOGIN_SERVER_H_ */
