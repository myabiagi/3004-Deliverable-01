#ifndef ORDERCONTROLLER_H
#define ORDERCONTROLLER_H

#include "sessionController.h"
#include "orderRepository.h"
#include "pricingService.h"
#include "customer.h"

#include <QString>
#include <memory>
#include <vector>

struct OrderPlacementResult {
    bool success = false;
    QString message;
    std::shared_ptr<Order> order = nullptr;
};

class OrderController {
public:
    OrderController(SessionController& sessionController,
                    OrderRepository& orderRepository,
                    PricingService& pricingService);

    // calculate checkout values without placing an order
    PriceBreakdown previewOnlineOrder(
        const std::vector<OrderItem>& items,
        int requestedLoyaltyPoints,
        qint64 requestedAccountCreditCents) const;

    // create and submit an online order for the active customer
    OrderPlacementResult placeOnlineOrder(
        const std::vector<OrderItem>& items,
        int requestedLoyaltyPoints,
        qint64 requestedAccountCreditCents);

private:
    // return the logged-in user as a customer
    std::shared_ptr<Customer> getCurrentCustomer() const;

    // ensure every selected item has a valid quantity and menu item
    bool areOrderItemsValid(const std::vector<OrderItem>& items) const;

    SessionController& sessionController;
    OrderRepository& orderRepository;
    PricingService& pricingService;
};

#endif
