#ifndef ORDERQUEUE_H
#define ORDERQUEUE_H

#include "orderRepository.h"

#include <cstddef>
#include <memory>
#include <vector>

// possible results of a kitchen completion request
enum class KitchenCompletionStatus {
  Completed,
  NotFound,
  NotNext,
  Failed
};

class OrderQueue {
public:
  // use the application's existing order repository
  explicit OrderQueue(OrderRepository& repository);

  // access the first queued order without removing it
  std::shared_ptr<Order> front() const;

  // find an order currently in the kitchen queue
  std::shared_ptr<Order> findByOrderNumber(int orderNumber) const;

  // retrieve all pending orders in fifo order
  std::vector<std::shared_ptr<Order>> getOrders() const;

  // access queue information
  bool isEmpty() const;
  std::size_t size() const;

  // complete only the first order and credit its earned loyalty points
  KitchenCompletionStatus completeFrontOrder(int orderNumber);

private:
  OrderRepository& repository;
};

#endif
