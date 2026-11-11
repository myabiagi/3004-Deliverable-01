#include "applicationController.h"

ApplicationController::ApplicationController(SessionController& sessionController)
    : sessionController(sessionController),
      currentWorkspace(WorkspaceType::Login) {
}

bool ApplicationController::loginAndSelectWorkspace(const QString& nameOrUsername) {
    if (!sessionController.login(nameOrUsername)) {
        currentWorkspace = WorkspaceType::Login;
        return false;
    }

    UserRole role;
    if (!sessionController.getCurrentUserRole(role)) {
        sessionController.logout();
        currentWorkspace = WorkspaceType::Login;
        return false;
    }

    currentWorkspace = workspaceForRole(role);
    return true;
}

void ApplicationController::logoutToLogin() {
    // logout clears the session only; defaults are not reloaded
    sessionController.logout();
    currentWorkspace = WorkspaceType::Login;
}

WorkspaceType ApplicationController::getCurrentWorkspace() const {
    return currentWorkspace;
}

bool ApplicationController::hasActiveUser() const {
    return sessionController.hasActiveUser();
}

std::shared_ptr<User> ApplicationController::getCurrentUser() const {
    return sessionController.getCurrentUser();
}

bool ApplicationController::getCurrentUserRole(UserRole& role) const {
    return sessionController.getCurrentUserRole(role);
}

WorkspaceType ApplicationController::workspaceForRole(UserRole role) const {
    switch (role) {
    case UserRole::Customer:
        return WorkspaceType::Customer;
    case UserRole::KitchenStaff:
        return WorkspaceType::Kitchen;
    case UserRole::Cashier:
    case UserRole::Manager:
    case UserRole::Administrator:
        return WorkspaceType::Staff;
    }

    return WorkspaceType::Login;
}
