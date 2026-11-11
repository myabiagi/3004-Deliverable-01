#include "order.h"
#include "customer.h"

Order::Order(int orderNumber, const std::shared_ptr<Customer>& customer,
             const QDateTime& placedAt, const std::vector<OrderItem>& items,
             qint64 subtotalCents, qint64 taxCents, qint64 totalCents,
             int loyaltyPointsUsed, qint64 accountCreditUsedCents,
             qint64 amountChargedCents, int loyaltyPointsEarned,
             OrderStatus status, OrderSource source)
    : orderNumber(orderNumber), customer(customer), placedAt(placedAt), items(items),
      subtotalCents(subtotalCents), taxCents(taxCents), totalCents(totalCents),
      loyaltyPointsUsed(loyaltyPointsUsed), accountCreditUsedCents(accountCreditUsedCents),
      amountChargedCents(amountChargedCents), loyaltyPointsEarned(loyaltyPointsEarned),
      status(status), source(source) {
}

int Order::getOrderNumber() const { return orderNumber; }
std::shared_ptr<Customer> Order::getCustomer() const { return customer; }
QString Order::getCustomerName() const { return customer ? customer->getName() : QString(); }
QString Order::getCustomerUsername() const { return customer ? customer->getUsername() : QString(); }
QDateTime Order::getPlacedAt() const { return placedAt; }
const std::vector<OrderItem>& Order::getItems() const { return items; }
qint64 Order::getSubtotalCents() const { return subtotalCents; }
qint64 Order::getTaxCents() const { return taxCents; }
qint64 Order::getTotalCents() const { return totalCents; }
int Order::getLoyaltyPointsUsed() const { return loyaltyPointsUsed; }
qint64 Order::getAccountCreditUsedCents() const { return accountCreditUsedCents; }
qint64 Order::getAmountChargedCents() const { return amountChargedCents; }
int Order::getLoyaltyPointsEarned() const { return loyaltyPointsEarned; }
OrderStatus Order::getStatus() const { return status; }
OrderSource Order::getSource() const { return source; }

bool Order::complete() {
    // d1 only permits the in progress to completed transition
    if (status != OrderStatus::InProgress) {
        return false;
    }
    status = OrderStatus::Completed;
    return true;
}

QString orderStatusToString(OrderStatus status) {
    switch (status) {
    case OrderStatus::InProgress: return "In Progress";
    case OrderStatus::Completed: return "Completed";
    case OrderStatus::PickedUp: return "Picked Up";
    case OrderStatus::Cancelled: return "Cancelled";
    }
    return "Unknown";
}

QString orderSourceToString(OrderSource source) {
    switch (source) {
    case OrderSource::Online: return "Online";
    case OrderSource::InPerson: return "In Person";
    }
    return "Unknown";
}