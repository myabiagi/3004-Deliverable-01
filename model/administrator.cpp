#include "administrator.h"

// initialize the inherited staff information
Administrator::Administrator(const QString& name,
                             const QString& username,
                             const QString& staffId)
    : Staff(name, username, staffId)
{
}

// return the administrator's user role
UserRole Administrator::getRole() const
{
    return UserRole::Administrator;
}
