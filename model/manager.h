#ifndef MANAGER_H
#define MANAGER_H

#include "staff.h"

class Manager : public Staff {
public:
    // create a manager with their name, username, and staff id
    Manager(const std::string& name,
            const std::string& username,
            const std::string& staffId);

    // identify this user as a manager
    UserRole getRole() const override;
};

#endif
