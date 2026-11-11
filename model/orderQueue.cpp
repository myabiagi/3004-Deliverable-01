#include "orderQueue.h"

#include <algorithm>

OrderQueue::OrderQueue(OrderRepository& repository) : repository(repository) {
}

std::shared_ptr<Order> OrderQueue::front() const {
    auto orders = getOrders();
    return orders.empty() ? nullptr : orders.front();
}

std::shared_ptr<Order> OrderQueue::findByOrderNumber(int orderNumber) const {
    auto order = repository.findByOrderNumber(orderNumber);
    // only paid, in-progress orders belong in the kitchen queue
    if (!order || order->getStatus() != OrderStatus::InProgress) {
        return nullptr;
    }
    return order;
}

std::vector<std::shared_ptr<Order>> OrderQueue::getOrders() const {
    std::vector<std::shared_ptr<Order>> queuedOrders;
    // retrieve in-progress orders, then order them FIFO by placement time
    for (const std::shared_ptr<Order>& order : repository.getAllOrders()) {
        if (order->getStatus() == OrderStatus::InProgress) {
            queuedOrders.push_back(order);
        }
    }

    std::sort(queuedOrders.begin(), queuedOrders.end(),
              [](const std::shared_ptr<Order>& a, const std::shared_ptr<Order>& b) {
                  if (a->getPlacedAt() == b->getPlacedAt()) {
                      return a->getOrderNumber() < b->getOrderNumber();
                  }
                  return a->getPlacedAt() < b->getPlacedAt();
              });

    return queuedOrders;
}

bool OrderQueue::isEmpty() const { return front() == nullptr; }
std::size_t OrderQueue::size() const { return getOrders().size(); }
