#include "kitchenStaff.h"

KitchenStaff::KitchenStaff(const QString& name, const QString& username, const QString& staffId)
  : Staff(name, username, staffId) {
}

UserRole KitchenStaff::getRole() const { return UserRole::KitchenStaff; }
