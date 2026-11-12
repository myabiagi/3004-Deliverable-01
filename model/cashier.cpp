#include "cashier.h"

Cashier::Cashier(const QString& name, const QString& username, const QString& staffId)
  : Staff(name, username, staffId) {
}

UserRole Cashier::getRole() const { return UserRole::Cashier; }
