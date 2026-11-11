#include "defaultDataLoader.h"

#include "administrator.h"
#include "cashier.h"
#include "customer.h"
#include "ingredient.h"
#include "ingredientRequirement.h"
#include "kitchenStaff.h"
#include "manager.h"
#include "menuCatalog.h"
#include "menuItem.h"
#include "order.h"
#include "orderItem.h"
#include "orderRepository.h"
#include "pricingService.h"
#include "userDirectory.h"

#include <QDateTime>
#include <memory>
#include <vector>

bool DefaultDataLoader::loadDefaults(UserDirectory& users, MenuCatalog& menu,
                                     OrderRepository& orders) const {
    // default data should only be created once per application run
    if (!users.getUsers().empty() || !menu.getAllItems().empty() || !orders.getAllOrders().empty()) {
        return false;
    }

    // load users and menu items before creating orders that use them
    return loadUsers(users) && loadMenu(menu) && loadOrderHistory(users, menu, orders);
}

bool DefaultDataLoader::loadUsers(UserDirectory& users) const {
    // balances include the effects of the five preloaded orders per customer
    // each customer starts with 30 points and 500 cents from earlier activity
    const int finalPoints[] = {33, 32, 33, 31, 31};
    const qint64 finalCreditCents[] = {1178, 300, 300, 300, 300};

    for (int i = 1; i <= 5; ++i) {
        auto customer = std::make_shared<Customer>(
            QString("Customer %1").arg(i),
            QString("customer%1").arg(i),
            QString("L%1").arg(1000 + i),
            QString("customer%1@example.com").arg(i),
            QString("%1 Campus Road").arg(i),
            finalPoints[i - 1], finalCreditCents[i - 1]);

        if (!users.addUser(customer)) {
            return false;
        }
    }

    // create the six required staff accounts
    return users.addUser(std::make_shared<KitchenStaff>("Kitchen One", "kitchen1", "K001")) &&
           users.addUser(std::make_shared<KitchenStaff>("Kitchen Two", "kitchen2", "K002")) &&
           users.addUser(std::make_shared<Cashier>("Cashier One", "cashier1", "C001")) &&
           users.addUser(std::make_shared<Cashier>("Cashier Two", "cashier2", "C002")) &&
           users.addUser(std::make_shared<Manager>("Manager One", "manager1", "M001")) &&
           users.addUser(std::make_shared<Administrator>("Admin One", "admin1", "A001"));
}

bool DefaultDataLoader::loadMenu(MenuCatalog& menu) const {
    // ingredients can be shared by multiple menu item recipes
    auto bread = std::make_shared<Ingredient>(1, "Bread");
    auto cheese = std::make_shared<Ingredient>(2, "Cheese");
    auto chicken = std::make_shared<Ingredient>(3, "Chicken");
    auto vegetables = std::make_shared<Ingredient>(4, "Vegetables");
    auto water = std::make_shared<Ingredient>(5, "Water");
    auto coffee = std::make_shared<Ingredient>(6, "Coffee");
    auto tea = std::make_shared<Ingredient>(7, "Tea");
    auto milk = std::make_shared<Ingredient>(8, "Milk");
    auto cocoa = std::make_shared<Ingredient>(9, "Cocoa");
    auto lemon = std::make_shared<Ingredient>(10, "Lemon");
    auto ice = std::make_shared<Ingredient>(11, "Ice");

    // create a menu item with its required ingredients and quantities
    auto addItem = [&menu](int id, const QString& name, MenuCategory category,
                           qint64 priceCents, const std::vector<IngredientRequirement>& recipe) {
        auto item = std::make_shared<MenuItem>(id, name, category, priceCents);
        for (const IngredientRequirement& requirement : recipe) {
            item->addIngredientRequirement(requirement);
        }
        return menu.addMenuItem(item);
    };

    // three sandwiches
    if (!addItem(1, "Cheese Sandwich", MenuCategory::Sandwich, 750,
                 {{bread, 2, "slices"}, {cheese, 2, "slices"}}) ||
        !addItem(2, "Chicken Sandwich", MenuCategory::Sandwich, 850,
                 {{bread, 2, "slices"}, {chicken, 100, "g"}}) ||
        !addItem(3, "Veggie Sandwich", MenuCategory::Sandwich, 800,
                 {{bread, 2, "slices"}, {vegetables, 1, "portion"}})) {
        return false;
    }

    // four hot beverages
    if (!addItem(4, "Coffee", MenuCategory::HotBeverage, 350,
                 {{coffee, 18, "g"}, {water, 240, "mL"}}) ||
        !addItem(5, "Tea", MenuCategory::HotBeverage, 300,
                 {{tea, 1, "bag"}, {water, 240, "mL"}}) ||
        !addItem(6, "Hot Chocolate", MenuCategory::HotBeverage, 425,
                 {{cocoa, 20, "g"}, {milk, 240, "mL"}}) ||
        !addItem(7, "Latte", MenuCategory::HotBeverage, 500,
                 {{coffee, 18, "g"}, {milk, 200, "mL"}})) {
        return false;
    }

    // three cold beverages
    return addItem(8, "Iced Coffee", MenuCategory::ColdBeverage, 475,
                   {{coffee, 18, "g"}, {milk, 180, "mL"}, {ice, 4, "cubes"}}) &&
           addItem(9, "Lemonade", MenuCategory::ColdBeverage, 450,
                   {{lemon, 1, "whole"}, {water, 240, "mL"}, {ice, 4, "cubes"}}) &&
           addItem(10, "Iced Tea", MenuCategory::ColdBeverage, 425,
                   {{tea, 1, "bag"}, {water, 240, "mL"}, {ice, 4, "cubes"}});
}

bool DefaultDataLoader::loadOrderHistory(UserDirectory& users, MenuCatalog& menu,
                                         OrderRepository& orders) const {
    PricingService pricingService;
    const QDateTime firstOrderDate = QDateTime::currentDateTime().addDays(-30);

    // each of the five customers starts with five historical orders
    for (int customerIndex = 1; customerIndex <= 5; ++customerIndex) {
        auto customer = std::dynamic_pointer_cast<Customer>(
            users.findByNameOrUsername(QString("customer%1").arg(customerIndex)));
        if (!customer) {
            return false;
        }

        for (int historyIndex = 0; historyIndex < 5; ++historyIndex) {
            // rotate through the menu to create different purchases
            int menuItemId = ((customerIndex + historyIndex - 1) % 10) + 1;
            auto menuItem = menu.findById(menuItemId);
            if (!menuItem) {
                return false;
            }

            std::vector<OrderItem> items;
            items.emplace_back(menuItem, 1);

            // only one historical order is cancelled and it uses both points and credit
            bool cancelled = (customerIndex == 1 && historyIndex == 1);
            int requestedPoints = (historyIndex == 1 || historyIndex == 2) ? 1 : 0;
            qint64 requestedCreditCents = (historyIndex == 1 || historyIndex == 3) ? 100 : 0;

            // calculate the recorded totals without changing customer balances
            PriceBreakdown price = pricingService.calculate(
                items, requestedPoints, requestedCreditCents, 30, 500);

            OrderStatus status = cancelled ? OrderStatus::Cancelled : OrderStatus::PickedUp;
            OrderSource source = (cancelled || historyIndex % 2 == 0)
                ? OrderSource::Online : OrderSource::InPerson;
            int earnedPoints = cancelled ? 0 : price.loyaltyPointsEarned;
            QDateTime placedAt = firstOrderDate.addDays(historyIndex * 5 + customerIndex - 1);

            auto order = std::make_shared<Order>(
                orders.generateOrderNumber(), customer, placedAt, items,
                price.subtotalCents, price.taxCents, price.totalCents,
                price.loyaltyPointsUsed, price.accountCreditUsedCents,
                price.amountChargedCents, earnedPoints, status, source);

            if (!orders.addOrder(order)) {
                return false;
            }
        }
    }

    return true;
}