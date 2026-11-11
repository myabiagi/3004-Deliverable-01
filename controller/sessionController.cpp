#include "sessionController.h"

SessionController::SessionController(UserDirectory& userDirectory)
    : userDirectory(userDirectory), currentUser(nullptr) {
}

bool SessionController::login(const QString& nameOrUsername) { // clear any previous session before trying to identify another user
    logout();

    std::shared_ptr<User> matchingUser =
        userDirectory.findByNameOrUsername(nameOrUsername);

    if (matchingUser == nullptr) {
        return false;
    }

    currentUser = matchingUser;
    return true;
}

void SessionController::logout() { // this ends the session but does not delete the user from the directory
    currentUser.reset();
}

bool SessionController::hasActiveUser() const {
    return currentUser != nullptr;
}

std::shared_ptr<User> SessionController::getCurrentUser() const {
    return currentUser;
}

bool SessionController::getCurrentUserRole(UserRole& role) const {
    if (currentUser == nullptr) {
        return false;
    }

    role = currentUser->getRole();
    return true;
}
