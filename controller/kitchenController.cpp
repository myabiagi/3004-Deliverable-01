#include "kitchenController.h"
#include "customer.h"

KitchenController::KitchenController(OrderQueue& orderQueue)
    : orderQueue(orderQueue) {
}

std::vector<std::shared_ptr<Order>> KitchenController::getQueuedOrders() const { // copy the orders while preserving their fifo order
    return orderQueue.getOrders();
}

std::shared_ptr<Order> KitchenController::getOrderDetails(int orderNumber) const {
    return orderQueue.findByOrderNumber(orderNumber);
}

KitchenActionResult KitchenController::completeOrder(int orderNumber) {
    KitchenActionResult result;

    std::shared_ptr<Order> order = orderQueue.findByOrderNumber(orderNumber);

    if (order == nullptr) {
        result.message = "The order was not found in the kitchen queue.";
        return result;
    }

    if (order->getStatus() != OrderStatus::InProgress) {
        result.message = "Only an in-progress order can be completed.";
        return result;
    }

    // kitchen works the FIFO queue from the front
    std::shared_ptr<Order> nextOrder = orderQueue.front();
    if (nextOrder == nullptr || nextOrder->getOrderNumber() != orderNumber) {
        result.message = "Only the next order in the queue can be completed.";
        return result;
    }

    // find the customer before changing the order
    std::shared_ptr<Customer> customer = order->getCustomer();

    if (customer == nullptr) {
        result.message = "The customer associated with the order was not found.";
        return result;
    }

    if (!order->complete()) {
        result.message = "The order could not be completed.";
        return result;
    }

    // points are credited when the order reaches completed
    customer->addLoyaltyPoints(order->getLoyaltyPointsEarned());

    result.success = true;
    result.message = "The order was completed successfully.";

    return result;
}
