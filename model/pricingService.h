#ifndef PRICINGSERVICE_H
#define PRICINGSERVICE_H

#include "orderItem.h"
#include <QtGlobal>
#include <vector>

struct PriceBreakdown {
    qint64 subtotalCents = 0;
    qint64 taxCents = 0;
    qint64 totalCents = 0;

    int loyaltyPointsUsed = 0;
    qint64 loyaltyValueCents = 0;
    int loyaltyPointsEarned = 0;

    qint64 accountCreditUsedCents = 0;
    qint64 amountChargedCents = 0;
};

class PricingService {
public:
    PriceBreakdown calculate(const std::vector<OrderItem>& items, int requestedLoyaltyPoints,
                             qint64 requestedAccountCreditCents, int availableLoyaltyPoints,
                             qint64 availableAccountCreditCents) const;

private:
    qint64 calculateSubtotal(const std::vector<OrderItem>& items) const;
    qint64 calculateTax(qint64 subtotalCents) const;
};

#endif