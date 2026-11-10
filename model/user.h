
#ifndef USER_H
#define USER_H

#include <string>

// The five user roles supported by DragonEndPOS.
enum class UserRole {
    Customer,
    KitchenStaff,
    Cashier,
    Manager,
    Administrator
};

class User {
public:
    // Create a user with their name and unique username.
    User(const std::string& name, const std::string& username);
    
    // Allow derived user objects to be destroyed correctly.
    virtual ~User() = default;

    // Access common user information.
    const std::string& getName() const;
    const std::string& getUsername() const;

    // Each concrete user type identifies its own role.
    virtual UserRole getRole() const = 0;

private:
    std::string name;
    std::string username;
};

#endif
