#include "ordercontroller.h"

#include <QDateTime>

OrderController::OrderController(
    SessionController& sessionController,
    OrderRepository& orderRepository,
    OrderQueue& orderQueue,
    PricingService& pricingService)
    : sessionController(sessionController),
      orderRepository(orderRepository),
      orderQueue(orderQueue),
      pricingService(pricingService)
{
}

PriceBreakdown OrderController::previewOnlineOrder(
    const std::vector<OrderItem>& items,
    int requestedLoyaltyPoints,
    qint64 requestedAccountCreditCents) const
{
    std::shared_ptr<Customer> customer =
        getCurrentCustomer();

    if (customer == nullptr || items.empty())
    {
        return PriceBreakdown();
    }

    return pricingService.calculate(
        items,
        requestedLoyaltyPoints,
        requestedAccountCreditCents,
        customer->getLoyaltyPoints(),
        customer->getAccountCreditCents());
}

OrderPlacementResult OrderController::placeOnlineOrder(
    const std::vector<OrderItem>& items,
    int requestedLoyaltyPoints,
    qint64 requestedAccountCreditCents)
{
    OrderPlacementResult result;

    std::shared_ptr<Customer> customer =
        getCurrentCustomer();

    if (customer == nullptr)
    {
        result.message = "A customer must be logged in.";
        return result;
    }

    // Only one current order for each customer
    if (customer->hasCurrentOrder())
    {
        result.message =
            "Only one order can be placed at a time";
        return result;
    }

    if (items.empty())
    {
        result.message =
            "The order must contain at least one item.";
        return result;
    }

    if (!areOrderItemsValid(items))
    {
        result.message =
            "The order contains an invalid item or quantity.";
        return result;
    }

    PriceBreakdown price =
        pricingService.calculate(
            items,
            requestedLoyaltyPoints,
            requestedAccountCreditCents,
            customer->getLoyaltyPoints(),
            customer->getAccountCreditCents());

    int orderNumber =
        orderRepository.generateOrderNumber();

    std::shared_ptr<Order> order =
        std::make_shared<Order>(
            orderNumber,
            customer->getName(),
            customer->getUsername(),
            QDateTime::currentDateTime(),
            items,
            price.subtotalCents,
            price.taxCents,
            price.totalCents,
            price.loyaltyPointsUsed,
            price.accountCreditUsedCents,
            price.amountChargedCents,
            price.loyaltyPointsEarned,
            OrderStatus::InProgress,
            OrderSource::Online);

    // these deductions are safe because pricingservice limits usage
    // to the customer's available balances
    if (!customer->deductLoyaltyPoints(
            price.loyaltyPointsUsed))
    {
        result.message =
            "The loyalty points could not be applied.";
        return result;
    }

    if (!customer->deductAccountCredit(
            price.accountCreditUsedCents))
    {
        // restore points if deducting account credit unexpectedly fails
        customer->addLoyaltyPoints(
            price.loyaltyPointsUsed);

        result.message =
            "The account credit could not be applied.";
        return result;
    }

    if (!orderRepository.addOrder(order))
    {
        // restore the balances because the order was not stored
        customer->addLoyaltyPoints(
            price.loyaltyPointsUsed);

        customer->addAccountCredit(
            price.accountCreditUsedCents);

        result.message =
            "The order could not be stored.";
        return result;
    }

    if (!orderQueue.enqueue(order))
    {
        // this should not occur for a valid in-progress order
        customer->addLoyaltyPoints(
            price.loyaltyPointsUsed);

        customer->addAccountCredit(
            price.accountCreditUsedCents);

        result.message =
            "The order could not be added to the kitchen queue.";
        return result;
    }

    if (!customer->setCurrentOrder(order))
    {
        orderQueue.removeByOrderNumber(orderNumber);

        customer->addLoyaltyPoints(
            price.loyaltyPointsUsed);

        customer->addAccountCredit(
            price.accountCreditUsedCents);

        result.message =
            "Only one order can be placed at a time";
        return result;
    }

    result.success = true;
    result.message = "The order was placed successfully.";
    result.order = order;

    return result;
}

std::shared_ptr<Customer>
OrderController::getCurrentCustomer() const
{
    std::shared_ptr<User> currentUser =
        sessionController.getCurrentUser();

    if (currentUser == nullptr)
    {
        return nullptr;
    }

    return std::dynamic_pointer_cast<Customer>(
        currentUser);
}

bool OrderController::areOrderItemsValid(
    const std::vector<OrderItem>& items) const
{
    for (const OrderItem& item : items)
    {
        if (item.getMenuItem() == nullptr ||
            item.getQuantity() <= 0)
        {
            return false;
        }
    }

    return true;
}
