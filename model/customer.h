#ifndef CUSTOMER_H
#define CUSTOMER_H

#include "user.h"

#include <QString>

class Customer : public User {
public:
  // create a customer with their account information
  Customer(const QString& name,
           const QString& username,
           const QString& loyaltyNumber,
           const QString& email,
           const QString& homeAddress,
           int loyaltyPoints,
           int accountCreditCents);

  // identify this user as a customer
  UserRole getRole() const override;

  // access customer account information
  QString getLoyaltyNumber() const;
  QString getEmail() const;
  QString getHomeAddress() const;
  int getLoyaltyPoints() const;
  int getAccountCreditCents() const;

  // update editable contact information
  bool setEmail(const QString& email);
  bool setHomeAddress(const QString& homeAddress);

  // manage loyalty points
  void addLoyaltyPoints(int points);
  bool deductLoyaltyPoints(int points);

  // manage account credit
  void addAccountCredit(int cents);
  bool deductAccountCredit(int cents);

private:
  QString loyaltyNumber;
  QString email;
  QString homeAddress;
  int loyaltyPoints;
  int accountCreditCents;
};

#endif
