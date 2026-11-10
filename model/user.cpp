#include "user.h"

// initialize the information shared by every user
User::User(const QString& name, const QString& username)
    : name(name),
      username(username)
{
}

// return the user's name
QString User::getName() const
{
    return name;
}

// return the user's unique username
QString User::getUsername() const
{
    return username;
}
