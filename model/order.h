#ifndef ORDER_H
#define ORDER_H

#include "orderItem.h"

#include <QDateTime>
#include <QString>
#include <memory>
#include <vector>

class Customer;
class OrderQueue;

// order states used by current and historical orders
enum class OrderStatus {
  InProgress,
  Completed,
  PickedUp,
  Cancelled
};

// distinguish online and in-person orders in historical records
enum class OrderSource {
  Online,
  InPerson
};

class Order {
public:
  // create an order with its customer, items and payment information
  Order(int orderNumber, const std::shared_ptr<Customer>& customer,
        const QDateTime& placedAt, const std::vector<OrderItem>& items,
        int subtotalCents, int taxCents, int totalCents,
        int loyaltyPointsUsed, int accountCreditUsedCents,
        int amountChargedCents, int loyaltyPointsEarned,
        OrderStatus status, OrderSource source);

  // access order identification and customer information
  int getOrderNumber() const;
  std::shared_ptr<Customer> getCustomer() const;
  QString getCustomerName() const;
  QString getCustomerUsername() const;
  QDateTime getPlacedAt() const;
  QString getPlacedAtDisplayString() const;

  // access purchased items and payment information
  const std::vector<OrderItem>& getItems() const;
  int getSubtotalCents() const;
  int getTaxCents() const;
  int getTotalCents() const;
  int getLoyaltyPointsUsed() const;
  int getAccountCreditUsedCents() const;
  int getAmountChargedCents() const;
  int getLoyaltyPointsEarned() const;
  bool isPaid() const;

  // access order status and source
  OrderStatus getStatus() const;
  OrderSource getSource() const;

  // validate purchased items
  static bool areItemsValid(const std::vector<OrderItem>& items);

private:
  // allow only the order queue to request order completion
  friend class OrderQueue;

  // complete an in-progress order
  bool complete();

  int orderNumber;
  std::shared_ptr<Customer> customer;
  QDateTime placedAt;
  std::vector<OrderItem> items;

  int subtotalCents;
  int taxCents;
  int totalCents;
  int loyaltyPointsUsed;
  int accountCreditUsedCents;
  int amountChargedCents;
  int loyaltyPointsEarned;

  bool paid;
  OrderStatus status;
  OrderSource source;
};

QString orderStatusToString(OrderStatus status);
QString orderSourceToString(OrderSource source);

#endif
