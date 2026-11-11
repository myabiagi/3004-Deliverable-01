#ifndef APPLICATIONCONTROLLER_H
#define APPLICATIONCONTROLLER_H

#include "sessionController.h"

#include <QString>
#include <memory>

// which UI surface MainWindow should show
enum class WorkspaceType {
    Login,
    Customer,
    Kitchen,
    Staff // cashier, manager, and administrator in D1
};

class ApplicationController {
public:
    explicit ApplicationController(SessionController& sessionController);

    // identify the user, then choose the matching workspace
    // returns false if the name/username is not found
    bool loginAndSelectWorkspace(const QString& nameOrUsername);

    // clear the session and return to the startup form
    void logoutToLogin();

    // currently selected workspace (Login when no active user)
    WorkspaceType getCurrentWorkspace() const;

    // convenience for the views
    bool hasActiveUser() const;
    std::shared_ptr<User> getCurrentUser() const;
    bool getCurrentUserRole(UserRole& role) const;

private:
    WorkspaceType workspaceForRole(UserRole role) const;

    SessionController& sessionController;
    WorkspaceType currentWorkspace;
};

#endif
