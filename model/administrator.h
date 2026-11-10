#ifndef ADMINISTRATOR_H
#define ADMINISTRATOR_H

#include "staff.h"

class Administrator : public Staff {
public:
    // create an administrator with their name, username, and staff id
    Administrator(const std::string& name,
                  const std::string& username,
                  const std::string& staffId);

    // identify this user as an administrator
    UserRole getRole() const override;
};

#endif
