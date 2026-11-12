#include "orderController.h"

#include <QDateTime>

OrderController::OrderController(SessionController& sessionController,
                                 OrderRepository& orderRepository,
                                 PricingService& pricingService)
  : sessionController(sessionController),
    orderRepository(orderRepository),
    pricingService(pricingService) {
}

PriceBreakdown OrderController::previewOnlineOrder(const std::vector<OrderItem>& items,
                                                   int requestedLoyaltyPoints,
                                                   int requestedAccountCreditCents) const {
  std::shared_ptr<Customer> customer = getCurrentCustomer();

  if (customer == nullptr) {
    return PriceBreakdown();
  }

  return pricingService.calculate(items, requestedLoyaltyPoints,
                                  requestedAccountCreditCents,
                                  customer->getLoyaltyPoints(),
                                  customer->getAccountCreditCents());
}

OrderPlacementResult OrderController::placeOnlineOrder(const std::vector<OrderItem>& items,
                                                       int requestedLoyaltyPoints,
                                                       int requestedAccountCreditCents) {
  OrderPlacementResult result;
  std::shared_ptr<Customer> customer = getCurrentCustomer();

  if (customer == nullptr) {
    result.message = "A customer must be logged in.";
    return result;
  }

  // report the one-current-order rule enforced by the repository
  if (orderRepository.hasCurrentOrder(customer->getUsername())) {
    result.message = "Only one order can be placed at a time";
    return result;
  }

  if (items.empty()) {
    result.message = "The order must contain at least one item.";
    return result;
  }

  if (!Order::areItemsValid(items)) {
    result.message = "The order contains an invalid item or quantity.";
    return result;
  }

  PriceBreakdown price = pricingService.calculate(items, requestedLoyaltyPoints,
                                                  requestedAccountCreditCents,
                                                  customer->getLoyaltyPoints(),
                                                  customer->getAccountCreditCents());

  if (!price.valid) {
    result.message = "The order could not be calculated.";
    return result;
  }

  // create the order from the values calculated by the model
  std::shared_ptr<Order> order = std::make_shared<Order>(
    orderRepository.generateOrderNumber(),
    customer,
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
    OrderSource::Online
  );

  // the repository handles balance changes and order storage
  if (!orderRepository.addOnlineOrder(order)) {
    result.message = "The order could not be placed.";
    return result;
  }

  result.success = true;
  result.message = "The order was placed successfully.";
  result.order = order;

  return result;
}

std::shared_ptr<Customer> OrderController::getCurrentCustomer() const {
  std::shared_ptr<User> currentUser = sessionController.getCurrentUser();

  if (currentUser == nullptr) {
    return nullptr;
  }

  return std::dynamic_pointer_cast<Customer>(currentUser);
}
