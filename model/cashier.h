#ifndef CASHIER_H
#define CASHIER_H

#include "staff.h"

class Cashier : public Staff {
public:
    // create a cashier with their name, username, and staff id
    Cashier(const std::string& name,
            const std::string& username,
            const std::string& staffId);

    // identify this user as a cashier
    UserRole getRole() const override;
};

#endif
