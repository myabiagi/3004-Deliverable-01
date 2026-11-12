#include "pricingService.h"
#include "order.h"

#include <algorithm>

PriceBreakdown PricingService::calculate(const std::vector<OrderItem>& items,
                                         int requestedLoyaltyPoints,
                                         int requestedAccountCreditCents,
                                         int availableLoyaltyPoints,
                                         int availableAccountCreditCents) const {
  PriceBreakdown result;

  if (!Order::areItemsValid(items)) {
    return result;
  }

  result.subtotalCents = calculateSubtotal(items);
  result.taxCents = calculateTax(result.subtotalCents);
  result.totalCents = result.subtotalCents + result.taxCents;

  // negative redemption requests are treated as zero
  requestedLoyaltyPoints = std::max(0, requestedLoyaltyPoints);
  requestedAccountCreditCents = std::max(0, requestedAccountCreditCents);
  availableLoyaltyPoints = std::max(0, availableLoyaltyPoints);
  availableAccountCreditCents = std::max(0, availableAccountCreditCents);

  // loyalty points cover at most half of the pre-tax subtotal
  int maximumPointsForOrder = result.subtotalCents / 200;

  result.loyaltyPointsUsed = std::min(requestedLoyaltyPoints, availableLoyaltyPoints);
  result.loyaltyPointsUsed = std::min(result.loyaltyPointsUsed, maximumPointsForOrder);

  // one loyalty point is worth one dollar
  result.loyaltyValueCents = result.loyaltyPointsUsed * 100;

  int remainingAfterPoints = result.totalCents - result.loyaltyValueCents;

  // account credit covers up to the remaining total
  result.accountCreditUsedCents = std::min(requestedAccountCreditCents, availableAccountCreditCents);
  result.accountCreditUsedCents = std::min(result.accountCreditUsedCents, remainingAfterPoints);

  result.amountChargedCents = remainingAfterPoints - result.accountCreditUsedCents;

  // earn one point per three dollars charged unless points were redeemed
  if (result.loyaltyPointsUsed == 0) {
    result.loyaltyPointsEarned = result.amountChargedCents / 300;
  }

  result.valid = true;
  return result;
}

int PricingService::calculateSubtotal(const std::vector<OrderItem>& items) const {
  int subtotal = 0;

  for (const OrderItem& item : items) {
    subtotal += item.getTotalPriceCents();
  }

  return subtotal;
}

int PricingService::calculateTax(int subtotalCents) const {
  // calculate 15 percent tax rounded to the nearest cent
  return (subtotalCents * 15 + 50) / 100;
}
