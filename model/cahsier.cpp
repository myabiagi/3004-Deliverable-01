
#include "cashier.h"

// initialize the inherited staff information
Cashier::Cashier(const std::string& name,
                 const std::string& username,
                 const std::string& staffId)
    : Staff(name, username, staffId) {
}

// return the cashier's user role
UserRole Cashier::getRole() const {
    return UserRole::Cashier;
}
