#include "customer.h"

// initialize the inherited user information and customer account details
Customer::Customer(const QString& name,
                   const QString& username,
                   const QString& loyaltyNumber,
                   const QString& email,
                   const QString& homeAddress,
                   int loyaltyPoints,
                   qint64 accountCreditCents)
    : User(name, username),
      loyaltyNumber(loyaltyNumber),
      email(email),
      homeAddress(homeAddress),
      loyaltyPoints(loyaltyPoints),
      accountCreditCents(accountCreditCents) {
}

// return the customer's user role
UserRole Customer::getRole() const {
    return UserRole::Customer;
}

// return the customer's loyalty number
QString Customer::getLoyaltyNumber() const {
    return loyaltyNumber;
}

// return the customer's email address
QString Customer::getEmail() const {
    return email;
}

// return the customer's home address
QString Customer::getHomeAddress() const {
    return homeAddress;
}

// return the customer's available loyalty points
int Customer::getLoyaltyPoints() const {
    return loyaltyPoints;
}

// return the customer's available account credit in cents
qint64 Customer::getAccountCreditCents() const {
    return accountCreditCents;
}

// update the customer's email address
void Customer::setEmail(const QString& email) {
    this->email = email;
}

// update the customer's home address
void Customer::setHomeAddress(const QString& homeAddress) {
    this->homeAddress = homeAddress;
}

// add a positive number of loyalty points
void Customer::addLoyaltyPoints(int points) {
    if (points > 0) {
        loyaltyPoints += points;
    }
}

// deduct loyalty points if the balance is sufficient
bool Customer::deductLoyaltyPoints(int points) {
    if (points < 0 || points > loyaltyPoints) {
        return false;
    }

    loyaltyPoints -= points;
    return true;
}

// add a positive amount of account credit
void Customer::addAccountCredit(qint64 cents) {
    if (cents > 0) {
        accountCreditCents += cents;
    }
}

// deduct account credit if the balance is sufficient
bool Customer::deductAccountCredit(qint64 cents) {
    if (cents < 0 || cents > accountCreditCents) {
        return false;
    }

    accountCreditCents -= cents;
    return true;
}
