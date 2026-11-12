#ifndef ORDERCONTROLLER_H
#define ORDERCONTROLLER_H

#include "customer.h"
#include "orderRepository.h"
#include "pricingService.h"
#include "sessionController.h"

#include <QString>
#include <memory>
#include <vector>

// result of an online order submission
struct OrderPlacementResult {
  bool success = false;
  QString message;
  std::shared_ptr<Order> order = nullptr;
};

class OrderController {
public:
  // use the existing session, order repository and pricing service
  OrderController(SessionController& sessionController,
                  OrderRepository& orderRepository,
                  PricingService& pricingService);

  // calculate checkout values without placing an order
  PriceBreakdown previewOnlineOrder(const std::vector<OrderItem>& items,
                                    int requestedLoyaltyPoints,
                                    int requestedAccountCreditCents) const;

  // submit an online order for the active customer
  OrderPlacementResult placeOnlineOrder(const std::vector<OrderItem>& items,
                                        int requestedLoyaltyPoints,
                                        int requestedAccountCreditCents);

private:
  // retrieve the active user as a customer
  std::shared_ptr<Customer> getCurrentCustomer() const;

  SessionController& sessionController;
  OrderRepository& orderRepository;
  PricingService& pricingService;
};

#endif
