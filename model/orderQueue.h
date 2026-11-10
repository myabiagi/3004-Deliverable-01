#ifndef ORDERQUEUE_H
#define ORDERQUEUE_H

#include "order.h"

#include <cstddef>
#include <deque>
#include <memory>

class OrderQueue
{
public:
    // add an in-progress order to the back of the queue
    bool enqueue(const std::shared_ptr<Order>& order);

    // return the first order without removing it
    std::shared_ptr<Order> front() const;

    // find a queued order using its order number
    std::shared_ptr<Order> findByOrderNumber(int orderNumber) const;

    // remove an order from the queue using its order number
    bool removeByOrderNumber(int orderNumber);

    // return all queued orders in fifo order
    const std::deque<std::shared_ptr<Order>>& getOrders() const;

    bool isEmpty() const;
    std::size_t size() const;

private:
    std::deque<std::shared_ptr<Order>> orders;
};

#endif
