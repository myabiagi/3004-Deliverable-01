
#ifndef CUSTOMER_H
#define CUSTOMER_H

#include "user.h"
#include <string>

class Customer : public User {
public:
    // create a customer with their account information
    Customer(const std::string& name,
             const std::string& username,
             const std::string& loyaltyNumber,
             const std::string& email,
             const std::string& homeAddress,
             int loyaltyPoints,
             int accountCreditCents); // used int to store account cents to avoid floating point rounding errors with double

    // identify this user as a customer
    UserRole getRole() const override;

    // access customer account information
    const std::string& getLoyaltyNumber() const;
    const std::string& getEmail() const;
    const std::string& getHomeAddress() const;
    int getLoyaltyPoints() const;
    int getAccountCreditCents() const;

    // update editable contact information
    void setEmail(const std::string& email);
    void setHomeAddress(const std::string& homeAddress);

    // manage loyalty points
    void addLoyaltyPoints(int points);
    bool deductLoyaltyPoints(int points);

    // manage account credit
    void addAccountCredit(int cents);
    bool deductAccountCredit(int cents);

private:
    std::string loyaltyNumber;
    std::string email;
    std::string homeAddress;
    int loyaltyPoints;
    int accountCreditCents;
};

#endif
