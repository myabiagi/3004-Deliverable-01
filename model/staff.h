
#ifndef STAFF_H
#define STAFF_H

#include "user.h"
#include <string>

class Staff : public User {
public:
    // create a staff member with their name, username, and staff id
    Staff(const std::string& name,
          const std::string& username,
          const std::string& staffId);

    // allow derived staff objects to be destroyed correctly
    virtual ~Staff() = default;

    // access the staff member's unique id
    const std::string& getStaffId() const;

private:
    std::string staffId;
};

#endif
