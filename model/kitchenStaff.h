#ifndef KITCHENSTAFF_H
#define KITCHENSTAFF_H

#include "staff.h"

class KitchenStaff : public Staff {
public:
    // create a kitchen staff member with their name, username, and staff id
    KitchenStaff(const std::string& name,
                 const std::string& username,
                 const std::string& staffId);

    // identify this user as kitchen staff
    UserRole getRole() const override;
};

#endif
