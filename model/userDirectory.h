#ifndef USERDIRECTORY_H
#define USERDIRECTORY_H

#include "user.h"

#include <QString>

#include <memory>
#include <vector>

class UserDirectory
{
public:
    // add a user if their username is unique
    bool addUser(const std::shared_ptr<User>& user);

    // find a user by name or username
    std::shared_ptr<User> findByNameOrUsername(
        const QString& input) const;

    // return all users without copying the vector
    const std::vector<std::shared_ptr<User>>&
    getUsers() const;

private:
    std::vector<std::shared_ptr<User>> users;
};

#endif
