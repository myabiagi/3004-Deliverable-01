#include "kitchenstaff.h"

// initialize the inherited staff information
KitchenStaff::KitchenStaff(const QString& name,
                           const QString& username,
                           const QString& staffId)
    : Staff(name, username, staffId)
{
}

// return the kitchen staff user role
UserRole KitchenStaff::getRole() const
{
    return UserRole::KitchenStaff;
}
