#include "orderRepository.h"
#include "customer.h"

#include <algorithm>

OrderRepository::OrderRepository() : nextOrderNumber(1) {
}

bool OrderRepository::addOrder(const std::shared_ptr<Order>& order) {
  if (order == nullptr || order->getOrderNumber() <= 0 ||
      order->getCustomer() == nullptr || !order->isPaid() ||
      !Order::areItemsValid(order->getItems())) {
    return false;
  }

  // order numbers must be unique
  if (findByOrderNumber(order->getOrderNumber()) != nullptr) {
    return false;
  }

  // customers may have only one current order
  bool isCurrent = order->getStatus() == OrderStatus::InProgress || order->getStatus() == OrderStatus::Completed;

  if (isCurrent && hasCurrentOrder(order->getCustomerUsername())) {
    return false;
  }

  orders.push_back(order);

  // keep generated numbers above historical order numbers
  if (order->getOrderNumber() >= nextOrderNumber) {
    nextOrderNumber = order->getOrderNumber() + 1;
  }

  return true;
}

bool OrderRepository::addOnlineOrder(const std::shared_ptr<Order>& order) {
  if (order == nullptr || order->getCustomer() == nullptr ||
      order->getSource() != OrderSource::Online ||
      order->getStatus() != OrderStatus::InProgress ||
      !order->isPaid()) {
    return false;
  }

  std::shared_ptr<Customer> customer = order->getCustomer();

  // validate the order before changing customer balances
  if (!Order::areItemsValid(order->getItems()) ||
      order->getLoyaltyPointsUsed() < 0 ||
      order->getAccountCreditUsedCents() < 0 ||
      order->getLoyaltyPointsUsed() > customer->getLoyaltyPoints() ||
      order->getAccountCreditUsedCents() > customer->getAccountCreditCents() ||
      hasCurrentOrder(customer->getUsername()) ||
      findByOrderNumber(order->getOrderNumber()) != nullptr) {
    return false;
  }

  if (!customer->deductLoyaltyPoints(order->getLoyaltyPointsUsed())) {
    return false;
  }

  // restore points if applying account credit fails
  if (!customer->deductAccountCredit(order->getAccountCreditUsedCents())) {
    customer->addLoyaltyPoints(order->getLoyaltyPointsUsed());
    return false;
  }

  // restore both balances if the order cannot be stored
  if (!addOrder(order)) {
    customer->addLoyaltyPoints(order->getLoyaltyPointsUsed());
    customer->addAccountCredit(order->getAccountCreditUsedCents());
    return false;
  }

  return true;
}

std::shared_ptr<Order> OrderRepository::findByOrderNumber(int orderNumber) const {
  for (const std::shared_ptr<Order>& order : orders) {
    if (order->getOrderNumber() == orderNumber) {
      return order;
    }
  }

  return nullptr;
}

std::shared_ptr<Order> OrderRepository::getCurrentOrder(const QString& customerUsername) const {
  for (const std::shared_ptr<Order>& order : orders) {
    bool sameCustomer = order->getCustomerUsername().compare(customerUsername, Qt::CaseInsensitive) == 0;

    bool isCurrent = order->getStatus() == OrderStatus::InProgress || order->getStatus() == OrderStatus::Completed;

    if (sameCustomer && isCurrent) {
      return order;
    }
  }

  return nullptr;
}

std::vector<std::shared_ptr<Order>> OrderRepository::getOrderHistory(const QString& customerUsername) const {
  std::vector<std::shared_ptr<Order>> history;

  for (const std::shared_ptr<Order>& order : orders) {
    bool sameCustomer = order->getCustomerUsername().compare(customerUsername, Qt::CaseInsensitive) == 0;

    bool isHistorical = order->getStatus() == OrderStatus::PickedUp || order->getStatus() == OrderStatus::Cancelled;

    if (sameCustomer && isHistorical) {
      history.push_back(order);
    }
  }

  // show the newest historical orders first
  std::sort(history.begin(), history.end(),
            [](const std::shared_ptr<Order>& a, const std::shared_ptr<Order>& b) {
              if (a->getPlacedAt() == b->getPlacedAt()) {
                return a->getOrderNumber() > b->getOrderNumber();
              }

              return a->getPlacedAt() > b->getPlacedAt();
            });

  // only return five without deleting older stored orders
  if (history.size() > 5) {
    history.resize(5);
  }

  return history;
}

bool OrderRepository::hasCurrentOrder(const QString& customerUsername) const {
  return getCurrentOrder(customerUsername) != nullptr;
}

int OrderRepository::generateOrderNumber() {
  int generatedNumber = nextOrderNumber;
  nextOrderNumber++;
  return generatedNumber;
}

const std::vector<std::shared_ptr<Order>>& OrderRepository::getAllOrders() const {
  return orders;
}
