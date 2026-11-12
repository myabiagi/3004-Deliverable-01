#include "order.h"
#include "customer.h"

Order::Order(int orderNumber, const std::shared_ptr<Customer>& customer,
             const QDateTime& placedAt, const std::vector<OrderItem>& items,
             int subtotalCents, int taxCents, int totalCents,
             int loyaltyPointsUsed, int accountCreditUsedCents,
             int amountChargedCents, int loyaltyPointsEarned,
             OrderStatus status, OrderSource source)
  : orderNumber(orderNumber),
    customer(customer),
    placedAt(placedAt),
    items(items),
    subtotalCents(subtotalCents),
    taxCents(taxCents),
    totalCents(totalCents),
    loyaltyPointsUsed(loyaltyPointsUsed),
    accountCreditUsedCents(accountCreditUsedCents),
    amountChargedCents(amountChargedCents),
    loyaltyPointsEarned(loyaltyPointsEarned),
    paid(true),
    status(status),
    source(source) {
}

int Order::getOrderNumber() const { return orderNumber; }
std::shared_ptr<Customer> Order::getCustomer() const { return customer; }
QString Order::getCustomerName() const { return customer ? customer->getName() : QString(); }
QString Order::getCustomerUsername() const { return customer ? customer->getUsername() : QString(); }
QDateTime Order::getPlacedAt() const { return placedAt; }
QString Order::getPlacedAtDisplayString() const { return placedAt.toString("yyyy-MM-dd:HH:mm"); }
const std::vector<OrderItem>& Order::getItems() const { return items; }
int Order::getSubtotalCents() const { return subtotalCents; }
int Order::getTaxCents() const { return taxCents; }
int Order::getTotalCents() const { return totalCents; }
int Order::getLoyaltyPointsUsed() const { return loyaltyPointsUsed; }
int Order::getAccountCreditUsedCents() const { return accountCreditUsedCents; }
int Order::getAmountChargedCents() const { return amountChargedCents; }
int Order::getLoyaltyPointsEarned() const { return loyaltyPointsEarned; }
OrderStatus Order::getStatus() const { return status; }
OrderSource Order::getSource() const { return source; }

bool Order::isPaid() const { return paid; }

bool Order::areItemsValid(const std::vector<OrderItem>& items) {
  if (items.empty()) {
    return false;
  }

  for (const OrderItem& item : items) {
    if (!item.isValid()) {
      return false;
    }
  }

  return true;
}

bool Order::complete() {
  // d1 only permits in-progress orders to become completed
  if (status != OrderStatus::InProgress || !paid) {
    return false;
  }

  status = OrderStatus::Completed;
  return true;
}

QString orderStatusToString(OrderStatus status) {
  switch (status) {
    case OrderStatus::InProgress:
      return "In Progress";

    case OrderStatus::Completed:
      return "Completed";

    case OrderStatus::PickedUp:
      return "Picked Up";

    case OrderStatus::Cancelled:
      return "Cancelled";
  }

  return "Unknown";
}

QString orderSourceToString(OrderSource source) {
  switch (source) {
    case OrderSource::Online:
      return "Online";

    case OrderSource::InPerson:
      return "In Person";
  }

  return "Unknown";
}
