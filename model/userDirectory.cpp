#include "userDirectory.h"
#include "staff.h"

const std::vector<std::shared_ptr<User>>& UserDirectory::getUsers() const {
  return users;
}

bool UserDirectory::addUser(const std::shared_ptr<User>& user) {
  if (user == nullptr) {
    return false;
  }

  QString newUsername = user->getUsername().trimmed();

  if (newUsername.isEmpty()) {
    return false;
  }

  // usernames must be unique regardless of capitalization
  for (const std::shared_ptr<User>& existingUser : users) {
    if (existingUser->getUsername().trimmed().compare(newUsername, Qt::CaseInsensitive) == 0) {
      return false;
    }
  }

  users.push_back(user);
  return true;
}

std::shared_ptr<User> UserDirectory::findByNameUsernameOrId(const QString& input) const {
  QString searchText = input.trimmed();

  if (searchText.isEmpty()) {
    return nullptr;
  }

  // check full names and usernames first
  for (const std::shared_ptr<User>& user : users) {
    bool nameMatches = user->getName().trimmed().compare(searchText, Qt::CaseInsensitive) == 0;
    bool usernameMatches = user->getUsername().trimmed().compare(searchText, Qt::CaseInsensitive) == 0;

    if (nameMatches || usernameMatches) {
      return user;
    }
  }

  // check staff ids for staff accounts
  for (const std::shared_ptr<User>& user : users) {
    std::shared_ptr<Staff> staff = std::dynamic_pointer_cast<Staff>(user);

    if (staff != nullptr &&
        staff->getStaffId().trimmed().compare(searchText, Qt::CaseInsensitive) == 0) {
      return user;
    }
  }

  return nullptr;
}