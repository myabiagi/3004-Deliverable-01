#include "user.h"

// initialize the information shared by every user
User::User(const QString& name, const QString& username)
  : name(name), username(username) {
}

QString User::getName() const { return name; }

QString User::getUsername() const { return username; }
