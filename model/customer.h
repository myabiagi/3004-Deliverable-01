#ifndef CUSTOMER_H
#define CUSTOMER_H

#include "user.h"

#include <QString>
#include <QtGlobal>

#include <memory>
#include <vector>

class Order; // forward declaration

class Customer : public User
{
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

    // manage the customer's current order
    bool hasCurrentOrder() const;
    std::shared_ptr<Order> getCurrentOrder() const;
    bool setCurrentOrder(const std::shared_ptr<Order>& order);

    // manage the customer's completed or cancelled order history
    const std::vector<std::shared_ptr<Order>>&
    getOrderHistory() const;

    bool addOrderToHistory(const std::shared_ptr<Order>& order);

private:
    QString loyaltyNumber;
    QString email;
    QString homeAddress;

    int loyaltyPoints;
    qint64 accountCreditCents;

    // points to the customer's one in-progress or completed order
    std::shared_ptr<Order> currentOrder;

    // stores the customer's picked-up and cancelled orders
    std::vector<std::shared_ptr<Order>> orderHistory;
};

#endif
