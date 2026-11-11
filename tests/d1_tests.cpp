#include "customerController.h"
#include "defaultDataLoader.h"
#include "kitchenController.h"
#include "menuCatalog.h"
#include "orderController.h"
#include "orderQueue.h"
#include "orderRepository.h"
#include "pricingService.h"
#include "sessionController.h"
#include "userDirectory.h"

#include <QCoreApplication>
#include <QDebug>
#include <memory>
#include <vector>

int main(int argc, char* argv[]) {
    QCoreApplication app(argc, argv);
    int passed = 0;
    int failed = 0;

    auto check = [&](bool condition, const QString& description) {
        if (condition) {
            ++passed;
            qInfo().noquote() << "PASS:" << description;
        } else {
            ++failed;
            qCritical().noquote() << "FAIL:" << description;
        }
    };

    UserDirectory users;
    MenuCatalog menu;
    OrderRepository orders;
    OrderQueue queue(orders);
    PricingService pricing;
    DefaultDataLoader loader;

    check(loader.loadDefaults(users, menu, orders), "default data loads");
    check(!loader.loadDefaults(users, menu, orders), "default data cannot load twice");
    check(users.getUsers().size() == 11, "11 users loaded");
    check(menu.getAllItems().size() == 10, "10 menu items loaded");
    check(orders.getAllOrders().size() == 25, "25 historical orders loaded");

    int cancelledCount = 0;
    int cancelledUsingBoth = 0;
    int inPersonCount = 0;
    for (const auto& order : orders.getAllOrders()) {
        if (order->getStatus() == OrderStatus::Cancelled) {
            ++cancelledCount;
            if (order->getLoyaltyPointsUsed() > 0 &&
                order->getAccountCreditUsedCents() > 0) {
                ++cancelledUsingBoth;
            }
        }
        if (order->getSource() == OrderSource::InPerson) {
            ++inPersonCount;
        }
    }
    check(cancelledCount >= 1 && cancelledUsingBoth >= 1,
          "history contains cancelled order using both points and credit");
    check(inPersonCount >= 1, "history records in-person purchase sources");
    check(queue.isEmpty(), "historical orders do not enter kitchen queue");

    SessionController session(users);
    CustomerController customerController(session, menu, orders);
    OrderController orderController(session, orders, pricing);
    KitchenController kitchenController(queue);

    for (int i = 1; i <= 5; ++i) {
        const QString username = QString("customer%1").arg(i);
        check(session.login(username), username + " can log in");
        check(customerController.getCurrentCustomer() != nullptr,
              username + " has customer role");
        check(customerController.getOrderHistory().size() == 5,
              username + " has five historical orders");
        check(customerController.getCurrentOrder() == nullptr,
              username + " has no current order initially");
    }

    check(!session.login("nonexistent"), "unknown login is rejected");
    check(session.login("customer1"), "customer1 logs in for checkout");
    auto customer = customerController.getCurrentCustomer();
    auto coffee = menu.findById(4);
    check(customer != nullptr && coffee != nullptr, "customer and coffee exist");
    if (!customer || !coffee) {
        qCritical() << "Cannot continue checkout tests";
        return 1;
    }

    std::vector<OrderItem> cart;
    cart.emplace_back(coffee, 2);
    PriceBreakdown preview = orderController.previewOnlineOrder(cart, 0, 0);
    check(preview.subtotalCents == 700, "two coffees cost 700 cents before tax");
    check(preview.taxCents == 105, "15 percent tax is 105 cents");
    check(preview.amountChargedCents == 805, "charged amount without redemptions is 805 cents");
    check(preview.loyaltyPointsEarned == 2, "805 cents earns two points");

    PriceBreakdown withPoints = orderController.previewOnlineOrder(cart, 99, 0);
    check(withPoints.loyaltyPointsUsed == 3, "loyalty redemption limited to half the pretax subtotal");
    check(withPoints.amountChargedCents == 505, "tax remains payable after redeeming points");
    check(withPoints.loyaltyPointsEarned == 0, "redeeming points prevents earning points");

    std::vector<OrderItem> invalidCart;
    invalidCart.emplace_back(coffee, 0);
    check(!orderController.placeOnlineOrder(invalidCart, 0, 0).success,
          "zero-quantity order is rejected");
    check(!orderController.placeOnlineOrder({}, 0, 0).success,
          "empty order is rejected");

    const int startingPoints = customer->getLoyaltyPoints();
    auto placed = orderController.placeOnlineOrder(cart, 0, 0);
    check(placed.success && placed.order != nullptr, "online order placed");
    if (!placed.success || !placed.order) {
        qCritical() << "Cannot continue kitchen tests";
        return 1;
    }

    check(placed.order->getSource() == OrderSource::Online, "new order source is online");
    check(placed.order->getStatus() == OrderStatus::InProgress, "new order starts in progress");
    check(customerController.getCurrentOrder() == placed.order, "dashboard retrieves current order");
    check(queue.size() == 1 && queue.front() == placed.order, "new order enters kitchen queue");
    check(customerController.getOrderHistory().size() == 5, "new order does not enter history");
    check(customer->getLoyaltyPoints() == startingPoints, "points are not awarded on placement");

    auto second = orderController.placeOnlineOrder(cart, 0, 0);
    check(!second.success && second.message == "Only one order can be placed at a time",
          "second current order is rejected with exact required message");

    auto completed = kitchenController.completeOrder(placed.order->getOrderNumber());
    check(completed.success, "kitchen completes the order");
    check(placed.order->getStatus() == OrderStatus::Completed, "status becomes completed");
    check(queue.isEmpty(), "completed order leaves kitchen queue");
    check(customerController.getCurrentOrder() == placed.order, "completed order remains current");
    check(customer->getLoyaltyPoints() == startingPoints + 2, "two earned points credited once");
    check(!kitchenController.completeOrder(placed.order->getOrderNumber()).success,
          "order cannot be completed twice");
    check(customer->getLoyaltyPoints() == startingPoints + 2,
          "repeated completion does not award points again");

    session.logout();
    check(!session.hasActiveUser(), "logout clears active session");
    check(session.login("customer1"), "customer can log back in");
    check(customerController.getCurrentOrder() == placed.order,
          "order survives logout and login during program run");

    qInfo().noquote() << QString("\nD1 tests: %1 passed, %2 failed").arg(passed).arg(failed);
    return failed == 0 ? 0 : 1;
}
