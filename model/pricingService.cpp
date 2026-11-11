#include "pricingService.h"
#include <algorithm>

PriceBreakdown PricingService::calculate(const std::vector<OrderItem>& items,
                                          int requestedLoyaltyPoints,
                                          qint64 requestedAccountCreditCents,
                                          int availableLoyaltyPoints,
                                          qint64 availableAccountCreditCents) const {
    PriceBreakdown result;
    result.subtotalCents = calculateSubtotal(items);
    result.taxCents = calculateTax(result.subtotalCents);
    result.totalCents = result.subtotalCents + result.taxCents;

    // negative payment values are treated as zero
    requestedLoyaltyPoints = std::max(0, requestedLoyaltyPoints);
    requestedAccountCreditCents = std::max<qint64>(0, requestedAccountCreditCents);
    availableLoyaltyPoints = std::max(0, availableLoyaltyPoints);
    availableAccountCreditCents = std::max<qint64>(0, availableAccountCreditCents);

    // loyalty points can cover at most half of the pre-tax subtotal
    int maximumPointsForOrder = static_cast<int>(result.subtotalCents / 200);
    result.loyaltyPointsUsed = std::min(requestedLoyaltyPoints, availableLoyaltyPoints);
    result.loyaltyPointsUsed = std::min(result.loyaltyPointsUsed, maximumPointsForOrder);

    // one loyalty point is worth one dollar
    result.loyaltyValueCents = static_cast<qint64>(result.loyaltyPointsUsed) * 100;
    qint64 remainingAfterPoints = result.totalCents - result.loyaltyValueCents;
    result.accountCreditUsedCents = std::min(requestedAccountCreditCents, availableAccountCreditCents);
    result.accountCreditUsedCents = std::min(result.accountCreditUsedCents, remainingAfterPoints);
    result.amountChargedCents = remainingAfterPoints - result.accountCreditUsedCents;

    if (result.loyaltyPointsUsed > 0) { // using any loyalty points prevents earning points on the order
        result.loyaltyPointsEarned = 0;
    } else { // one point is earned for every three dollars charged
        result.loyaltyPointsEarned = static_cast<int>(result.amountChargedCents / 300);
    }
    return result;
}

qint64 PricingService::calculateSubtotal(const std::vector<OrderItem>& items) const {
    qint64 subtotalCents = 0;
    for (const OrderItem& item : items) {
        subtotalCents += item.getTotalPriceCents();
    }
    return subtotalCents;
}

qint64 PricingService::calculateTax(qint64 subtotalCents) const {
    return (subtotalCents * 15 + 50) / 100; // add 50 before division to round the tax to the nearest cent
}