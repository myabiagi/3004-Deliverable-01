#include "customer.h"

// initialize the inherited user information and customer account details
Customer::Customer(const QString& name,
                   const QString& username,
                   const QString& loyaltyNumber,
                   const QString& email,
                   const QString& homeAddress,
                   int loyaltyPoints,
                   int accountCreditCents)
  : User(name, username),
    loyaltyNumber(loyaltyNumber),
    email(email),
    homeAddress(homeAddress),
    loyaltyPoints(loyaltyPoints),
    accountCreditCents(accountCreditCents) {
}

UserRole Customer::getRole() const { return UserRole::Customer; }

// access customer account information
QString Customer::getLoyaltyNumber() const { return loyaltyNumber; }
QString Customer::getEmail() const { return email; }
QString Customer::getHomeAddress() const { return homeAddress; }
int Customer::getLoyaltyPoints() const { return loyaltyPoints; }
int Customer::getAccountCreditCents() const { return accountCreditCents; }

// update editable contact information
bool Customer::setEmail(const QString& email) {
  QString trimmedEmail = email.trimmed();

  if (trimmedEmail.isEmpty()) {
    return false;
  }

  this->email = trimmedEmail;
  return true;
}

bool Customer::setHomeAddress(const QString& homeAddress) {
  QString trimmedAddress = homeAddress.trimmed();

  if (trimmedAddress.isEmpty()) {
    return false;
  }

  this->homeAddress = trimmedAddress;
  return true;
}

// add or deduct loyalty points
void Customer::addLoyaltyPoints(int points) {
  if (points > 0) {
    loyaltyPoints += points;
  }
}

bool Customer::deductLoyaltyPoints(int points) {
  if (points < 0 || points > loyaltyPoints) {
    return false;
  }

  loyaltyPoints -= points;
  return true;
}

// add or deduct account credit
void Customer::addAccountCredit(int cents) {
  if (cents > 0) {
    accountCreditCents += cents;
  }
}

bool Customer::deductAccountCredit(int cents) {
  if (cents < 0 || cents > accountCreditCents) {
    return false;
  }

  accountCreditCents -= cents;
  return true;
}
