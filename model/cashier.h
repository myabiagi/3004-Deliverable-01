#ifndef CASHIER_H
#define CASHIER_H

#include "staff.h"

#include <QString>

class Cashier : public Staff
{
public:
    // create a cashier with their name, username, and staff id
    Cashier(const QString& name,
            const QString& username,
            const QString& staffId);

    // identify this user as a cashier
    UserRole getRole() const override;
};

#endif
