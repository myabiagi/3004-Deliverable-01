
#include "staff.h"

// initialize the inherited user information and staff id
Staff::Staff(const std::string& name,
             const std::string& username,
             const std::string& staffId)
    : User(name, username),
      staffId(staffId) {
}

// return the staff member's unique id
const std::string& Staff::getStaffId() const {
    return staffId;
}
