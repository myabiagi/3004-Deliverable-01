#include "cashier.h"

// initialize the inherited staff information
Cashier::Cashier(const QString& name,
                 const QString& username,
                 const QString& staffId)
    : Staff(name, username, staffId)
{
}

// return the cashier's user role
UserRole Cashier::getRole() const
{
    return UserRole::Cashier;
}
