#ifndef USERDIRECTORY_H
#define USERDIRECTORY_H

#include "user.h"

#include <memory>
#include <vector>

class UserDirectory {
public:
  // add a user if their username is unique
  bool addUser(const std::shared_ptr<User>& user);

  // find a user by their full name, username or staff id
  std::shared_ptr<User> findByNameUsernameOrId(const QString& input) const;

  // access all registered users without copying the collection
  const std::vector<std::shared_ptr<User>>& getUsers() const;

private:
  std::vector<std::shared_ptr<User>> users;
};

#endif
