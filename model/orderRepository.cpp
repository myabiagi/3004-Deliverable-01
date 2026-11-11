#include "orderRepository.h"
#include <algorithm>

OrderRepository::OrderRepository() : nextOrderNumber(1) {}

bool OrderRepository::addOrder(const std::shared_ptr<Order>& order) {
    if (!order || order->getOrderNumber() <= 0 || !order->getCustomer()) {
        return false;
    }

    // prevent two orders from using the same order number
    if (findByOrderNumber(order->getOrderNumber()) != nullptr) {
        return false;
    }

    // prevent customers from having more than one current order
    if ((order->getStatus() == OrderStatus::InProgress ||
         order->getStatus() == OrderStatus::Completed) &&
        hasCurrentOrder(order->getCustomerUsername())) {
        return false;
    }

    orders.push_back(order);

    // keep future generated numbers above manually loaded order numbers
    if (order->getOrderNumber() >= nextOrderNumber) {
        nextOrderNumber = order->getOrderNumber() + 1;
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

std::vector<std::shared_ptr<Order>> OrderRepository::getOrderHistory(
    const QString& customerUsername) const {
    std::vector<std::shared_ptr<Order>> history;
    for (const std::shared_ptr<Order>& order : orders) {
        bool sameCustomer = order->getCustomerUsername().compare(customerUsername, Qt::CaseInsensitive) == 0;

        bool isHistorical = order->getStatus() == OrderStatus::PickedUp || order->getStatus() == OrderStatus::Cancelled;
        
        if (sameCustomer && isHistorical) {
            history.push_back(order);
        }
    }

    // sort historical orders from newest to oldest
    std::sort(history.begin(), history.end(),
              [](const std::shared_ptr<Order>& a, const std::shared_ptr<Order>& b) {
                  return a->getPlacedAt() > b->getPlacedAt();
              });
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