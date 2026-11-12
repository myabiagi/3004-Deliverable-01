#include "orderQueue.h"
#include "customer.h"

#include <algorithm>

OrderQueue::OrderQueue(OrderRepository& repository)
  : repository(repository) {
}

std::shared_ptr<Order> OrderQueue::front() const {
  std::vector<std::shared_ptr<Order>> orders = getOrders();

  return orders.empty() ? nullptr : orders.front();
}

std::shared_ptr<Order> OrderQueue::findByOrderNumber(int orderNumber) const {
  std::shared_ptr<Order> order = repository.findByOrderNumber(orderNumber);

  // only in-progress orders belong in the kitchen queue
  if (order == nullptr || order->getStatus() != OrderStatus::InProgress) {
    return nullptr;
  }

  return order;
}

std::vector<std::shared_ptr<Order>> OrderQueue::getOrders() const {
  std::vector<std::shared_ptr<Order>> queuedOrders;

  // collect orders that are still being prepared
  for (const std::shared_ptr<Order>& order : repository.getAllOrders()) {
    if (order->getStatus() == OrderStatus::InProgress) {
      queuedOrders.push_back(order);
    }
  }

  // process the oldest order first
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

KitchenCompletionStatus OrderQueue::completeFrontOrder(int orderNumber) {
  std::shared_ptr<Order> order = findByOrderNumber(orderNumber);

  if (order == nullptr) {
    return KitchenCompletionStatus::NotFound;
  }

  // only the oldest queued order may be completed
  std::shared_ptr<Order> nextOrder = front();

  if (nextOrder == nullptr || nextOrder->getOrderNumber() != orderNumber) {
    return KitchenCompletionStatus::NotNext;
  }

  std::shared_ptr<Customer> customer = order->getCustomer();

  if (customer == nullptr || !order->complete()) {
    return KitchenCompletionStatus::Failed;
  }

  // earned points are credited only when the order is completed
  customer->addLoyaltyPoints(order->getLoyaltyPointsEarned());

  return KitchenCompletionStatus::Completed;
}
