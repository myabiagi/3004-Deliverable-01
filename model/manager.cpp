#include "manager.h"

// initialize the inherited staff information
Manager::Manager(const std::string& name,
                 const std::string& username,
                 const std::string& staffId)
    : Staff(name, username, staffId) {
}

// return the manager's user role
UserRole Manager::getRole() const {
    return UserRole::Manager;
}
