
#ifndef SESSIONCONTROLLER_H
#define SESSIONCONTROLLER_H

#include "userdirectory.h"

#include <QString>
#include <memory>

class SessionController
{
public:
    // use the application's existing user directory
    explicit SessionController(UserDirectory& userDirectory);

    // identify a user by their name or username
    bool login(const QString& nameOrUsername);

    // end the current user session
    void logout();

    // return whether a user is currently logged in
    bool hasActiveUser() const;

    // return the currently logged-in user
    std::shared_ptr<User> getCurrentUser() const;

    // retrieve the active user's role
    bool getCurrentUserRole(UserRole& role) const;

private:
    UserDirectory& userDirectory;
    std::shared_ptr<User> currentUser;
};

#endif
