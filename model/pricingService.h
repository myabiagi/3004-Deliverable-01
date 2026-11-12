#ifndef PRICINGSERVICE_H
#define PRICINGSERVICE_H

#include "orderItem.h"

#include <vector>

// amounts calculated for an online order
struct PriceBreakdown {
  int subtotalCents = 0;
  int taxCents = 0;
  int totalCents = 0;

  int loyaltyPointsUsed = 0;
  int loyaltyValueCents = 0;
  int loyaltyPointsEarned = 0;

  int accountCreditUsedCents = 0;
  int amountChargedCents = 0;

  bool valid = false;
};

class PricingService {
public:
  // calculate order amounts using available points and account credit
  PriceBreakdown calculate(const std::vector<OrderItem>& items,
                           int requestedLoyaltyPoints, int requestedAccountCreditCents,
                           int availableLoyaltyPoints, int availableAccountCreditCents) const;

private:
  // calculate the cost before discounts and tax
  int calculateSubtotal(const std::vector<OrderItem>& items) const;

  // apply the d1 tax rate to the full subtotal
  int calculateTax(int subtotalCents) const;
};

#endif
