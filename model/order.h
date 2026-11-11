#ifndef ORDER_H
#define ORDER_H

#include <QString>
#include <QDateTime>
#include <QtGlobal>
#include <memory>
#include <vector>
#include "orderItem.h"

class Customer;

enum class OrderStatus {
    InProgress,
    Completed,
    PickedUp,
    Cancelled
};

enum class OrderSource {
    Online,
    InPerson
};

class Order {
public:
    Order(int orderNumber, const std::shared_ptr<Customer>& customer,
          const QDateTime& placedAt, const std::vector<OrderItem>& items,
          qint64 subtotalCents, qint64 taxCents, qint64 totalCents,
          int loyaltyPointsUsed, qint64 accountCreditUsedCents,
          qint64 amountChargedCents, int loyaltyPointsEarned,
          OrderStatus status, OrderSource source);

    int getOrderNumber() const;
    std::shared_ptr<Customer> getCustomer() const;
    QString getCustomerName() const;
    QString getCustomerUsername() const;
    QDateTime getPlacedAt() const;
    const std::vector<OrderItem>& getItems() const;
    qint64 getSubtotalCents() const;
    qint64 getTaxCents() const;
    qint64 getTotalCents() const;
    int getLoyaltyPointsUsed() const;
    qint64 getAccountCreditUsedCents() const;
    qint64 getAmountChargedCents() const;
    int getLoyaltyPointsEarned() const;
    OrderStatus getStatus() const;
    OrderSource getSource() const;
    bool complete();

private:
    int orderNumber;
    std::shared_ptr<Customer> customer;
    QDateTime placedAt;
    std::vector<OrderItem> items;
    qint64 subtotalCents;
    qint64 taxCents;
    qint64 totalCents;
    int loyaltyPointsUsed;
    qint64 accountCreditUsedCents;
    qint64 amountChargedCents;
    int loyaltyPointsEarned;
    OrderStatus status;
    OrderSource source;
};

QString orderStatusToString(OrderStatus status);
QString orderSourceToString(OrderSource source);

#endif