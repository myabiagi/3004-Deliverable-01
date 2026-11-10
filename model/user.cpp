#include "user.h"

// Initialize the information shared by every user.
User::User(const std::string& name, const std::string& username)
    : name(name), username(username) {
}

// Return the user's name.
const std::string& User::getName() const {
    return name;
}

// Return the user's unique username.
const std::string& User::getUsername() const {
    return username;
}
