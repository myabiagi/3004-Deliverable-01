#ifndef CUSTOMER_H
#define CUSTOMER_H

#include "user.h"

#include <QString>
#include <QtGlobal>

class Customer : public User {
public:
    // create a customer with their account information
    Customer(const QString& name,
             const QString& username,
             const QString& loyaltyNumber,
             const QString& email,
             const QString& homeAddress,
             int loyaltyPoints,
             qint64 accountCreditCents);

    // identify this user as a customer
    UserRole getRole() const override;

    // access customer account information
    QString getLoyaltyNumber() const;
    QString getEmail() const;
    QString getHomeAddress() const;
    int getLoyaltyPoints() const;
    qint64 getAccountCreditCents() const;

    // update editable contact information
    void setEmail(const QString& email);
    void setHomeAddress(const QString& homeAddress);

    // manage loyalty points
    void addLoyaltyPoints(int points);
    bool deductLoyaltyPoints(int points);

    // manage account credit
    void addAccountCredit(qint64 cents);
    bool deductAccountCredit(qint64 cents);

private:
    QString loyaltyNumber;
    QString email;
    QString homeAddress;

    int loyaltyPoints;
    qint64 accountCreditCents;
};

#endif
