#ifndef KITCHENSTAFF_H
#define KITCHENSTAFF_H

#include "staff.h"

class KitchenStaff : public Staff {
public:
  // create a kitchen staff member with their name, username and staff id
  KitchenStaff(const QString& name, const QString& username, const QString& staffId);

  // identify this user as kitchen staff
  UserRole getRole() const override;
};

#endif
