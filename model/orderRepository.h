#ifndef ORDERREPOSITORY_H
#define ORDERREPOSITORY_H

#include "order.h"

#include <QString>
#include <memory>
#include <vector>

class OrderRepository {
public:
  OrderRepository();

  // store an order if its number and customer state are valid
  bool addOrder(const std::shared_ptr<Order>& order);

  // store a new online order and apply its payment balances
  bool addOnlineOrder(const std::shared_ptr<Order>& order);

  // find an order by its unique number
  std::shared_ptr<Order> findByOrderNumber(int orderNumber) const;

  // retrieve a customer's current or historical orders
  std::shared_ptr<Order> getCurrentOrder(const QString& customerUsername) const;
  std::vector<std::shared_ptr<Order>> getOrderHistory(const QString& customerUsername) const;

  // check whether a customer already has a current order
  bool hasCurrentOrder(const QString& customerUsername) const;

  // generate the next unique order number
  int generateOrderNumber();

  // access all stored orders without copying the collection
  const std::vector<std::shared_ptr<Order>>& getAllOrders() const;

private:
  std::vector<std::shared_ptr<Order>> orders;
  int nextOrderNumber;
};

#endif
