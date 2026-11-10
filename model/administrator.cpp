#include "administrator.h"

// initialize the inherited staff information
Administrator::Administrator(const std::string& name,
                             const std::string& username,
                             const std::string& staffId)
    : Staff(name, username, staffId) {
}

// return the administrator's user role
UserRole Administrator::getRole() const {
    return UserRole::Administrator;
}
