#ifndef STAFF_H
#define STAFF_H

#include "user.h"

#include <QString>

class Staff : public User
{
public:
    // create a staff member with their name, username, and staff id
    Staff(const QString& name,
          const QString& username,
          const QString& staffId);

    // allow derived staff objects to be destroyed correctly
    ~Staff() override = default;

    // access the staff member's unique id
    QString getStaffId() const;

private:
    QString staffId;
};

#endif
