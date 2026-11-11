#ifndef ORDERQUEUE_H
#define ORDERQUEUE_H

#include "orderRepository.h"
#include <cstddef>
#include <memory>
#include <vector>

class OrderQueue {
public:
    explicit OrderQueue(OrderRepository& repository);
    // return the first order without removing it
    std::shared_ptr<Order> front() const;
    // find a queued order using its order number
    std::shared_ptr<Order> findByOrderNumber(int orderNumber) const;
    // return all queued orders in fifo order
    std::vector<std::shared_ptr<Order>> getOrders() const;
    bool isEmpty() const;
    std::size_t size() const;

private:
    OrderRepository& repository;
};

#endif