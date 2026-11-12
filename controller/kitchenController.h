#ifndef KITCHENCONTROLLER_H
#define KITCHENCONTROLLER_H

#include "orderQueue.h"

#include <QString>
#include <memory>
#include <vector>

// result returned to the view after a kitchen action
struct KitchenActionResult {
  bool success = false;
  QString message;
};

class KitchenController {
public:
  // use the application's existing kitchen queue
  explicit KitchenController(OrderQueue& orderQueue);

  // retrieve pending orders in fifo order
  std::vector<std::shared_ptr<Order>> getQueuedOrders() const;

  // retrieve the details of any queued order
  std::shared_ptr<Order> getOrderDetails(int orderNumber) const;

  // request completion of a queued order
  KitchenActionResult completeOrder(int orderNumber);

private:
  OrderQueue& orderQueue;
};

#endif
