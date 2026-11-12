#ifndef STAFF_H
#define STAFF_H

#include "user.h"

class Staff : public User {
public:
  // create a staff member with their name, username and staff id
  Staff(const QString& name, const QString& username, const QString& staffId);

  // access the staff member's unique id
  QString getStaffId() const;

private:
  QString staffId;
};

#endif
