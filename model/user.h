#ifndef USER_H
#define USER_H

#include <QString>

// the five user roles supported by dragonendpos
enum class UserRole {
  Customer,
  KitchenStaff,
  Cashier,
  Manager,
  Administrator
};

class User {
public:
  // create a user with their name and unique username
  User(const QString& name, const QString& username);

  // allow derived user objects to be destroyed correctly
  virtual ~User() = default;

  // access common user information
  QString getName() const;
  QString getUsername() const;

  // each concrete user type identifies its own role
  virtual UserRole getRole() const = 0;

private:
  QString name;
  QString username;
};

#endif
