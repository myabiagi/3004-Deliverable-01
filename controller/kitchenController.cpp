#include "kitchencontroller.h"
#include "customer.h"

KitchenController::KitchenController(
    UserDirectory& userDirectory,
    OrderQueue& orderQueue)
    : userDirectory(userDirectory),
      orderQueue(orderQueue)
{
}

std::vector<std::shared_ptr<Order>>
KitchenController::getQueuedOrders() const
{
    std::vector<std::shared_ptr<Order>> queuedOrders;

    // copy the orders while preserving their fifo order
    for (const std::shared_ptr<Order>& order : orderQueue.getOrders())
    {
        queuedOrders.push_back(order);
    }

    return queuedOrders;
}

std::shared_ptr<Order>
KitchenController::getOrderDetails(int orderNumber) const
{
    return orderQueue.findByOrderNumber(orderNumber);
}

KitchenActionResult
KitchenController::completeOrder(int orderNumber)
{
    KitchenActionResult result;

    std::shared_ptr<Order> order =
        orderQueue.findByOrderNumber(orderNumber);

    if (order == nullptr)
    {
        result.message =
            "The order was not found in the kitchen queue.";
        return result;
    }

    if (order->getStatus() != OrderStatus::InProgress)
    {
        result.message =
            "Only an in-progress order can be completed.";
        return result;
    }

    // find the customer before changing the order
    std::shared_ptr<User> user =
        userDirectory.findByNameOrUsername(
            order->getCustomerUsername());

    std::shared_ptr<Customer> customer =
        std::dynamic_pointer_cast<Customer>(user);

    if (customer == nullptr)
    {
        result.message =
            "The customer associated with the order was not found.";
        return result;
    }

    if (!order->complete())
    {
        result.message =
            "The order could not be completed.";
        return result;
    }

    // remove completed orders from the kitchen queue
    if (!orderQueue.removeByOrderNumber(orderNumber))
    {
        result.message =
            "The completed order could not be removed from the queue.";
        return result;
    }

    // points are credited when the order reaches completed
    customer->addLoyaltyPoints(
        order->getLoyaltyPointsEarned());

    result.success = true;
    result.message =
        "The order was completed successfully.";

    return result;
}
