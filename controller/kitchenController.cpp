#include "kitchenController.h"

KitchenController::KitchenController(OrderQueue& orderQueue)
  : orderQueue(orderQueue) {
}

std::vector<std::shared_ptr<Order>> KitchenController::getQueuedOrders() const {
  return orderQueue.getOrders();
}
std::shared_ptr<Order> KitchenController::getOrderDetails(int orderNumber) const {
  return orderQueue.findByOrderNumber(orderNumber);
}

KitchenActionResult KitchenController::completeOrder(int orderNumber) {
  KitchenActionResult result;

  // the model enforces fifo ordering and handles completion
  KitchenCompletionStatus status = orderQueue.completeFrontOrder(orderNumber);

  switch (status) {
    case KitchenCompletionStatus::Completed:
      result.success = true;
      result.message = "The order was completed successfully.";
      break;

    case KitchenCompletionStatus::NotFound:
      result.message = "The order was not found in the kitchen queue.";
      break;

    case KitchenCompletionStatus::NotNext:
      result.message = "Only the next order in the queue can be completed.";
      break;

    case KitchenCompletionStatus::Failed:
      result.message = "The order could not be completed.";
      break;
  }

  return result;
}
