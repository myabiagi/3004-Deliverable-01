#include "applicationController.h"
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
#include <QRegularExpression>
#include <QStringList>

#include <algorithm>
#include <memory>
#include <vector>

namespace {

constexpr int kStartingLoyaltyPoints = 30;
constexpr qint64 kStartingAccountCreditCents = 500;

void replayHistoryBalances(const std::vector<std::shared_ptr<Order>>& history,
                           int& expectedPoints,
                           qint64& expectedCreditCents) {
    expectedPoints = kStartingLoyaltyPoints;
    expectedCreditCents = kStartingAccountCreditCents;

    std::vector<std::shared_ptr<Order>> chronological = history;
    std::sort(chronological.begin(), chronological.end(),
              [](const std::shared_ptr<Order>& a, const std::shared_ptr<Order>& b) {
                  return a->getPlacedAt() < b->getPlacedAt();
              });

    for (const auto& order : chronological) {
        if (order->getStatus() == OrderStatus::Cancelled) {
            // net effect of place + cancel: points unchanged, credit increases by amount charged
            expectedCreditCents += order->getAmountChargedCents();
        } else {
            expectedPoints -= order->getLoyaltyPointsUsed();
            expectedCreditCents -= order->getAccountCreditUsedCents();
            expectedPoints += order->getLoyaltyPointsEarned();
        }
    }
}

} // namespace

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

    const QStringList customerUsernames = {
        "elara.brightshield",
        "finnick.stormrider",
        "mira.willowstep",
        "cedric.ashborne",
        "lyra.moonwhisper"
    };

    check(loader.loadDefaults(users, menu, orders), "default data loads");
    check(!loader.loadDefaults(users, menu, orders), "default data cannot load twice");
    check(users.getUsers().size() == 11, "11 users loaded");
    check(menu.getAllItems().size() == 10, "10 menu items loaded");
    check(menu.getItemsByCategory(MenuCategory::Sandwich).size() >= 3,
          "at least 3 sandwiches loaded");
    check(menu.getItemsByCategory(MenuCategory::HotBeverage).size() >= 4,
          "at least 4 hot beverages loaded");
    check(menu.getItemsByCategory(MenuCategory::ColdBeverage).size() >= 3,
          "at least 3 cold beverages loaded");
    check(orders.getAllOrders().size() == 25, "25 historical orders loaded");

    int cancelledCount = 0;
    int cancelledUsingBoth = 0;
    int historyUsedPoints = 0;
    int historyUsedCredit = 0;
    int historyEarnedPoints = 0;
    int inPersonCount = 0;
    for (const auto& order : orders.getAllOrders()) {
        if (order->getStatus() == OrderStatus::Cancelled) {
            ++cancelledCount;
            if (order->getLoyaltyPointsUsed() > 0 &&
                order->getAccountCreditUsedCents() > 0) {
                ++cancelledUsingBoth;
            }
        }
        if (order->getLoyaltyPointsUsed() > 0) {
            ++historyUsedPoints;
        }
        if (order->getAccountCreditUsedCents() > 0) {
            ++historyUsedCredit;
        }
        if (order->getLoyaltyPointsEarned() > 0) {
            ++historyEarnedPoints;
        }
        if (order->getSource() == OrderSource::InPerson) {
            ++inPersonCount;
        }
        check(QRegularExpression("^\\d{4}-\\d{2}-\\d{2}:\\d{2}:\\d{2}$")
                  .match(order->getPlacedAtDisplayString())
                  .hasMatch(),
              QString("order %1 uses ISO display timestamp").arg(order->getOrderNumber()));
    }
    check(cancelledCount >= 1 && cancelledUsingBoth >= 1,
          "history contains cancelled order using both points and credit");
    check(historyUsedPoints >= 1, "history contains an order that used loyalty points");
    check(historyUsedCredit >= 1, "history contains an order that used account credit");
    check(historyEarnedPoints >= 1, "history contains an order that earned loyalty points");
    check(inPersonCount >= 1, "history records in-person purchase sources");
    check(queue.isEmpty(), "historical orders do not enter kitchen queue");

    SessionController session(users);
    ApplicationController application(session);
    CustomerController customerController(session, menu, orders);
    OrderController orderController(session, orders, pricing);
    KitchenController kitchenController(queue);

    check(application.getCurrentWorkspace() == WorkspaceType::Login,
          "application starts on the login workspace");
    check(!application.loginAndSelectWorkspace("nonexistent"),
          "application rejects unknown login");
    check(application.getCurrentWorkspace() == WorkspaceType::Login,
          "failed login leaves the login workspace");

    check(application.loginAndSelectWorkspace("elara.brightshield"),
          "Elara login selects a workspace");
    check(application.getCurrentWorkspace() == WorkspaceType::Customer,
          "customer login opens the customer workspace");
    application.logoutToLogin();
    check(!application.hasActiveUser(), "application logout clears the session");
    check(application.getCurrentWorkspace() == WorkspaceType::Login,
          "application logout returns to the login workspace");

    check(application.loginAndSelectWorkspace("timmy.tortoise"),
          "Timmy Tortoise login selects a workspace");
    check(application.getCurrentWorkspace() == WorkspaceType::Kitchen,
          "kitchen login opens the kitchen workspace");
    application.logoutToLogin();

    check(application.loginAndSelectWorkspace("harold.hare"),
          "Harold Hare login selects a workspace");
    check(application.getCurrentWorkspace() == WorkspaceType::Staff,
          "cashier login opens the staff workspace");
    application.logoutToLogin();

    check(application.loginAndSelectWorkspace("briar.stonekettle"),
          "Briar Stonekettle login selects a workspace");
    check(application.getCurrentWorkspace() == WorkspaceType::Staff,
          "manager login opens the staff workspace");
    application.logoutToLogin();

    check(application.loginAndSelectWorkspace("sage.ledgerkeep"),
          "Sage Ledgerkeep login selects a workspace");
    check(application.getCurrentWorkspace() == WorkspaceType::Staff,
          "administrator login opens the staff workspace");
    application.logoutToLogin();

    for (const QString& username : customerUsernames) {
        check(session.login(username), username + " can log in");
        check(customerController.getCurrentCustomer() != nullptr,
              username + " has customer role");
        check(customerController.getOrderHistory().size() == 5,
              username + " has five historical orders");
        check(customerController.getCurrentOrder() == nullptr,
              username + " has no current order initially");

        auto loadedCustomer = customerController.getCurrentCustomer();
        int expectedPoints = 0;
        qint64 expectedCredit = 0;
        replayHistoryBalances(customerController.getOrderHistory(),
                              expectedPoints, expectedCredit);
        check(loadedCustomer->getLoyaltyPoints() == expectedPoints,
              username + " loyalty balance matches order history");
        check(loadedCustomer->getAccountCreditCents() == expectedCredit,
              username + " account credit matches order history");
        check(!loadedCustomer->getLoyaltyNumber().isEmpty(),
              username + " has a loyalty points number");
        check(loadedCustomer->getEmail().contains("@"),
              username + " has an email address");
        check(!loadedCustomer->getHomeAddress().isEmpty(),
              username + " has a home address");
    }

    check(!session.login("nonexistent"), "unknown login is rejected");
    check(session.login("elara.brightshield"), "Elara logs in for checkout");
    auto customer = customerController.getCurrentCustomer();
    auto brew = menu.findById(4);
    check(customer != nullptr && brew != nullptr, "customer and hot brew exist");
    if (!customer || !brew) {
        qCritical() << "Cannot continue checkout tests";
        return 1;
    }

    auto emailUpdate = customerController.updateEmail("elara.brightshield@adventurers.ca");
    check(emailUpdate.success && customer->getEmail() == "elara.brightshield@adventurers.ca",
          "customer can update email");
    auto addressUpdate = customerController.updateHomeAddress("99 Adventure Lane, Hintonville");
    check(addressUpdate.success &&
              customer->getHomeAddress() == "99 Adventure Lane, Hintonville",
          "customer can update home address");
    check(!customerController.updateEmail("   ").success, "empty email is rejected");
    check(!customerController.updateHomeAddress("").success, "empty home address is rejected");

    std::vector<OrderItem> cart;
    cart.emplace_back(brew, 2);
    PriceBreakdown preview = orderController.previewOnlineOrder(cart, 0, 0);
    check(preview.subtotalCents == 700, "two brews cost 700 cents before tax");
    check(preview.taxCents == 105, "15 percent tax is 105 cents");
    check(preview.amountChargedCents == 805, "charged amount without redemptions is 805 cents");
    check(preview.loyaltyPointsEarned == 2, "805 cents earns two points");

    PriceBreakdown withPoints = orderController.previewOnlineOrder(cart, 99, 0);
    check(withPoints.loyaltyPointsUsed == 3, "loyalty redemption limited to half the pretax subtotal");
    check(withPoints.amountChargedCents == 505, "tax remains payable after redeeming points");
    check(withPoints.loyaltyPointsEarned == 0, "redeeming points prevents earning points");

    PriceBreakdown withCredit = orderController.previewOnlineOrder(cart, 0, 300);
    check(withCredit.accountCreditUsedCents == 300, "account credit can be applied");
    check(withCredit.amountChargedCents == 505, "credit reduces the amount charged");
    check(withCredit.loyaltyPointsEarned == 1, "points are earned only on the amount charged");

    std::vector<OrderItem> invalidCart;
    invalidCart.emplace_back(brew, 0);
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
    check(QRegularExpression("^\\d{4}-\\d{2}-\\d{2}:\\d{2}:\\d{2}$")
              .match(placed.order->getPlacedAtDisplayString())
              .hasMatch(),
          "new order uses ISO display timestamp");

    auto second = orderController.placeOnlineOrder(cart, 0, 0);
    check(!second.success && second.message == "Only one order can be placed at a time",
          "second current order is rejected with exact required message");

    // second customer places so the kitchen queue has two FIFO entries
    session.logout();
    check(session.login("finnick.stormrider"), "Finnick logs in for second queue order");
    auto secondCustomer = customerController.getCurrentCustomer();
    check(secondCustomer != nullptr, "Finnick is available");
    auto placed2 = orderController.placeOnlineOrder(cart, 0, 0);
    check(placed2.success && placed2.order != nullptr, "Finnick online order placed");
    if (!placed2.success || !placed2.order) {
        qCritical() << "Cannot continue FIFO kitchen tests";
        return 1;
    }

    auto queued = kitchenController.getQueuedOrders();
    check(queued.size() == 2, "kitchen queue holds two in-progress orders");
    check(queued.size() == 2 && queued[0] == placed.order && queued[1] == placed2.order,
          "kitchen queue is ordered FIFO by placement time");
    check(!kitchenController.completeOrder(placed2.order->getOrderNumber()).success,
          "kitchen cannot complete an order that is not at the front");

    auto completed = kitchenController.completeOrder(placed.order->getOrderNumber());
    check(completed.success, "kitchen completes the front order");
    check(placed.order->getStatus() == OrderStatus::Completed, "status becomes completed");
    check(queue.size() == 1 && queue.front() == placed2.order,
          "completed order leaves kitchen queue and next order advances");
    check(session.login("elara.brightshield"), "Elara session restored after kitchen completion");
    check(customerController.getCurrentOrder() == placed.order, "completed order remains current");
    check(customer->getLoyaltyPoints() == startingPoints + 2, "two earned points credited once");
    check(!kitchenController.completeOrder(placed.order->getOrderNumber()).success,
          "order cannot be completed twice");
    check(customer->getLoyaltyPoints() == startingPoints + 2,
          "repeated completion does not award points again");

    auto completedSecond = kitchenController.completeOrder(placed2.order->getOrderNumber());
    check(completedSecond.success, "kitchen completes the next FIFO order");
    check(queue.isEmpty(), "kitchen queue is empty after both completions");

    session.logout();
    check(!session.hasActiveUser(), "logout clears active session");
    check(session.login("elara.brightshield"), "customer can log back in");
    check(customerController.getCurrentOrder() == placed.order,
          "order survives logout and login during program run");

    qInfo().noquote() << QString("\nD1 tests: %1 passed, %2 failed").arg(passed).arg(failed);
    return failed == 0 ? 0 : 1;
}
