#ifndef ORDERREPOSITORY_H
#define ORDERREPOSITORY_H

#include "order.h"
#include <QString>
#include <memory>
#include <vector>

class OrderRepository {
public:
    OrderRepository();

    // add an order if its order number is unique
    bool addOrder(const std::shared_ptr<Order>& order);

    // find an order using its unique order number
    std::shared_ptr<Order> findByOrderNumber(int orderNumber) const;

    // find a customer's in-progress or completed order
    std::shared_ptr<Order> getCurrentOrder(const QString& customerUsername) const;

    // return the customer's five most recent picked-up or cancelled orders
    std::vector<std::shared_ptr<Order>> getOrderHistory(const QString& customerUsername) const;

    // check whether the customer already has a current order
    bool hasCurrentOrder(const QString& customerUsername) const;

    // generate a unique order number for a new order
    int generateOrderNumber();

    // return all stored orders without copying the vector
    const std::vector<std::shared_ptr<Order>>& getAllOrders() const;

private:
    std::vector<std::shared_ptr<Order>> orders;
    int nextOrderNumber;
};

#endif