#include "servers/login_server.h"
#include "libnutrition/data/user.h"

namespace LoginServer {

  LogInResponse doLogin(const LogInRequest& req, int& loggedInUserId)
  {
    LogInResponse resp;
    QSharedPointer<User> user;
    bool success = false;
    std::string error;

    loggedInUserId = -1;

    if (req.has_username()) {

      user = User::getUserByUsername(QString::fromStdString(req.username()));

      if (user) {

        if (req.createuser()) {

          error = "Username already exists";

        } else if (req.has_password_sha1_hex()) {

          if (user->checkPasswordHash(QString::fromStdString(req.password_sha1_hex()))) {
            success = true;
          } else {
            error = "Incorrect password";
          }

        } else {
          error = "No password given";
        }

      } else if (req.createuser()) {

        QString errorMessage;

        user =
          User::createUserWithPasswordHash
            (QString::fromStdString(req.username()),
             QString::fromStdString(req.realname()),
             QString::fromStdString(req.password_sha1_hex()),
             errorMessage);

        if (user) {
          success = true;
        } else {
          error = errorMessage.toStdString();
        }

      } else {
        error = "Invalid username";
      }

    } else {
      error = "No username given";
    }

    if (error != "") {

      resp.mutable_error()->set_iserror(true);
      resp.mutable_error()->set_errormessage(error);

    } else if (success && user) {

      loggedInUserId = user->getId();
      resp.set_userid(loggedInUserId);
      resp.set_username(user->getUsername().toStdString());
      resp.set_realname(user->getRealName().toStdString());

      User::setLoggedInUserId(loggedInUserId);

    } else {

      // Shouldn't get here, but just in case...
      resp.mutable_error()->set_iserror(true);
    }

    return resp;
  }
}
