#include "kitchenstaff.h"

// initialize the inherited staff information
KitchenStaff::KitchenStaff(const std::string& name,
                           const std::string& username,
                           const std::string& staffId)
    : Staff(name, username, staffId) {
}

// return the kitchen staff user role
UserRole KitchenStaff::getRole() const {
    return UserRole::KitchenStaff;
}
