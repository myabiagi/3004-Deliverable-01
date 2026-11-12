#include "staff.h"

// initialize the inherited user information and staff id
Staff::Staff(const QString& name, const QString& username, const QString& staffId)
  : User(name, username), staffId(staffId) {
}

QString Staff::getStaffId() const { return staffId; }
