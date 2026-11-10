#ifndef MANAGER_H
#define MANAGER_H

#include "staff.h"

#include <QString>

class Manager : public Staff
{
public:
    // create a manager with their name, username, and staff id
    Manager(const QString& name,
            const QString& username,
            const QString& staffId);

    // identify this user as a manager
    UserRole getRole() const override;
};

#endif
