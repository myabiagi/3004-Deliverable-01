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

namespace {
// starting balances before applying the historical orders
const int startingLoyaltyPoints = 30;
const int startingAccountCreditCents = 500;

struct CustomerSeed {
  const char* name;
  const char* username;
  const char* loyaltyNumber;
  const char* email;
  const char* homeAddress;
};

// five customer accounts with unique identification information
const CustomerSeed customerSeeds[] = {
  {"Elara Brightshield", "elara.brightshield", "LP-48219",
   "elara.brightshield@hintonville.ca", "14 Willow Lane, Hintonville"},
  {"Finnick Stormrider", "finnick.stormrider", "LP-57304",
   "finnick.stormrider@hintonville.ca", "8 Ember Court, Hintonville"},
  {"Mira Willowstep", "mira.willowstep", "LP-61988",
   "mira.willowstep@hintonville.ca", "27 Oakbridge Road, Hintonville"},
  {"Cedric Ashborne", "cedric.ashborne", "LP-70412",
   "cedric.ashborne@hintonville.ca", "3 Stonekettle Way, Hintonville"},
  {"Lyra Moonwhisper", "lyra.moonwhisper", "LP-82567",
   "lyra.moonwhisper@hintonville.ca", "19 Glacier Path, Hintonville"}
};

const int customerCount = sizeof(customerSeeds) / sizeof(customerSeeds[0]);

bool applyPickedUpEffects(Customer& customer, const PriceBreakdown& price) {
  // completed historical purchases used their redeemed balances
  if (!customer.deductLoyaltyPoints(price.loyaltyPointsUsed) ||
      !customer.deductAccountCredit(price.accountCreditUsedCents)) {
    return false;
  }

  customer.addLoyaltyPoints(price.loyaltyPointsEarned);
  return true;
}

bool applyCancelledEffects(Customer& customer, const PriceBreakdown& price) {
  // reconstruct the original payment before applying the cancellation
  if (!customer.deductLoyaltyPoints(price.loyaltyPointsUsed) ||
      !customer.deductAccountCredit(price.accountCreditUsedCents)) {
    return false;
  }

  // restore redeemed balances and refund the charged amount as account credit
  customer.addLoyaltyPoints(price.loyaltyPointsUsed);
  customer.addAccountCredit(price.accountCreditUsedCents);
  customer.addAccountCredit(price.amountChargedCents);

  // cancelled orders do not retain earned loyalty points
  return true;
}

}

bool DefaultDataLoader::loadDefaults(UserDirectory& users, MenuCatalog& menu, OrderRepository& orders) const {
  // default data should only be created once
  if (!users.getUsers().empty() || !menu.getAllItems().empty() ||
      !orders.getAllOrders().empty()) {
    return false;
  }

  // load users and menu items before creating orders that reference them
  return loadUsers(users) &&
         loadMenu(menu) &&
         loadOrderHistory(users, menu, orders);
}

bool DefaultDataLoader::loadUsers(UserDirectory& users) const {
  // customers begin with balances from activity before the displayed history
  for (int i = 0; i < customerCount; ++i) {
    const CustomerSeed& seed = customerSeeds[i];

    std::shared_ptr<Customer> customer = std::make_shared<Customer>(
      QString::fromUtf8(seed.name),
      QString::fromUtf8(seed.username),
      QString::fromUtf8(seed.loyaltyNumber),
      QString::fromUtf8(seed.email),
      QString::fromUtf8(seed.homeAddress),
      startingLoyaltyPoints,
      startingAccountCreditCents
    );

    if (!users.addUser(customer)) {
      return false;
    }
  }

  // two kitchen staff accounts
  if (!users.addUser(std::make_shared<KitchenStaff>(
        "Simon Siju", "simon.siju", "K-1001")) ||
      !users.addUser(std::make_shared<KitchenStaff>(
        "Rowan Emberpot", "rowan.emberpot", "K-1002"))) {
    return false;
  }

  // two cashier accounts
  if (!users.addUser(std::make_shared<Cashier>(
        "Ishaan Bahl", "ishaan.bahl", "C-2001")) ||
      !users.addUser(std::make_shared<Cashier>(
        "Nora Quicktill", "nora.quicktill", "C-2002"))) {
    return false;
  }

  // one manager and one administrator
  return users.addUser(std::make_shared<Manager>(
           "Briar Stonekettle", "briar.stonekettle", "M-3001")) &&
         users.addUser(std::make_shared<Administrator>(
           "Sage Ledgerkeep", "sage.ledgerkeep", "A-4001"));
}

bool DefaultDataLoader::loadMenu(MenuCatalog& menu) const {
  // ingredients can be shared across multiple menu item recipes
  auto bread = std::make_shared<Ingredient>(1, "Sourdough Bread");
  auto cheese = std::make_shared<Ingredient>(2, "Aged Cheddar");
  auto chicken = std::make_shared<Ingredient>(3, "Roasted Chicken");
  auto vegetables = std::make_shared<Ingredient>(4, "Garden Vegetables");
  auto water = std::make_shared<Ingredient>(5, "Spring Water");
  auto coffee = std::make_shared<Ingredient>(6, "Dark Roast Coffee");
  auto tea = std::make_shared<Ingredient>(7, "Black Tea Leaves");
  auto milk = std::make_shared<Ingredient>(8, "Steamed Milk");
  auto cocoa = std::make_shared<Ingredient>(9, "Cocoa Powder");
  auto lemon = std::make_shared<Ingredient>(10, "Fresh Lemon");
  auto ice = std::make_shared<Ingredient>(11, "Crushed Ice");

  // create a menu item with its required ingredients and quantities
  auto addItem = [&menu](int id, const QString& name, MenuCategory category,
                         int priceCents, const std::vector<IngredientRequirement>& recipe) {
    auto item = std::make_shared<MenuItem>(id, name, category, priceCents);

    for (const IngredientRequirement& requirement : recipe) {
      item->addIngredientRequirement(requirement);
    }

    return menu.addMenuItem(item);
  };

  // three sandwiches
  if (!addItem(1, "Mount Doom Melt", MenuCategory::Sandwich, 750,
               {{bread, 2, "slices"}, {cheese, 2, "slices"}}) ||
      !addItem(2, "Dragonfire Chicken Club", MenuCategory::Sandwich, 850,
               {{bread, 2, "slices"}, {chicken, 100, "g"}}) ||
      !addItem(3, "Verdant Trail Veggie", MenuCategory::Sandwich, 800,
               {{bread, 2, "slices"}, {vegetables, 1, "portion"}})) {
    return false;
  }

  // four hot beverages
  if (!addItem(4, "Hintonville Brew", MenuCategory::HotBeverage, 350,
               {{coffee, 18, "g"}, {water, 240, "mL"}}) ||
      !addItem(5, "Sorcerer's Chai", MenuCategory::HotBeverage, 300,
               {{tea, 1, "bag"}, {water, 240, "mL"}}) ||
      !addItem(6, "Ember Cocoa", MenuCategory::HotBeverage, 425,
               {{cocoa, 20, "g"}, {milk, 240, "mL"}}) ||
      !addItem(7, "Adventurer's Latte", MenuCategory::HotBeverage, 500,
               {{coffee, 18, "g"}, {milk, 200, "mL"}})) {
    return false;
  }

  // three cold beverages
  return addItem(8, "Glacier Iced Coffee", MenuCategory::ColdBeverage, 475,
                 {{coffee, 18, "g"}, {milk, 180, "mL"}, {ice, 4, "cubes"}}) &&
         addItem(9, "Sunlit Lemonade", MenuCategory::ColdBeverage, 450,
                 {{lemon, 1, "whole"}, {water, 240, "mL"}, {ice, 4, "cubes"}}) &&
         addItem(10, "Shadowleaf Iced Tea", MenuCategory::ColdBeverage, 425,
                 {{tea, 1, "bag"}, {water, 240, "mL"}, {ice, 4, "cubes"}});
}

bool DefaultDataLoader::loadOrderHistory(UserDirectory& users, MenuCatalog& menu, OrderRepository& orders) const {
  PricingService pricingService;
  const QDateTime firstOrderDate = QDateTime::currentDateTime().addDays(-30);

  // create five historical orders for each of the five customers
  for (int customerIndex = 0; customerIndex < customerCount; ++customerIndex) {
    std::shared_ptr<Customer> customer = std::dynamic_pointer_cast<Customer>(
      users.findByNameUsernameOrId(QString::fromUtf8(customerSeeds[customerIndex].username))
    );

    if (customer == nullptr) {
      return false;
    }

    for (int historyIndex = 0; historyIndex < 5; ++historyIndex) {
      // rotate through the menu to create different purchases
      int menuItemId = ((customerIndex + historyIndex) % 10) + 1;
      std::shared_ptr<MenuItem> menuItem = menu.findById(menuItemId);

      if (menuItem == nullptr) {
        return false;
      }

      std::vector<OrderItem> items;
      items.emplace_back(menuItem, 1);

      // elara's second order demonstrates cancellation with points and credit
      bool cancelled = (customerIndex == 0 && historyIndex == 1);

      int requestedPoints = (historyIndex == 1 || historyIndex == 2) ? 1 : 0;
      int requestedCreditCents = (historyIndex == 1 || historyIndex == 3) ? 100 : 0;

      // use each customer's balance at this point in their history
      PriceBreakdown price = pricingService.calculate(
        items,
        requestedPoints,
        requestedCreditCents,
        customer->getLoyaltyPoints(),
        customer->getAccountCreditCents()
      );

      if (!price.valid) {
        return false;
      }

      OrderStatus status = cancelled ? OrderStatus::Cancelled : OrderStatus::PickedUp;

      OrderSource source = (cancelled || historyIndex % 2 == 0)
        ? OrderSource::Online
        : OrderSource::InPerson;

      int earnedPoints = cancelled ? 0 : price.loyaltyPointsEarned;
      QDateTime placedAt = firstOrderDate.addDays(historyIndex * 5 + customerIndex);

      std::shared_ptr<Order> order = std::make_shared<Order>(
        orders.generateOrderNumber(),
        customer,
        placedAt,
        items,
        price.subtotalCents,
        price.taxCents,
        price.totalCents,
        price.loyaltyPointsUsed,
        price.accountCreditUsedCents,
        price.amountChargedCents,
        earnedPoints,
        status,
        source
      );

      if (!orders.addOrder(order)) {
        return false;
      }

      // reconstruct the balance effects of the recorded history
      bool balancesUpdated = cancelled
        ? applyCancelledEffects(*customer, price)
        : applyPickedUpEffects(*customer, price);

      if (!balancesUpdated) {
        return false;
      }
    }
  }

  return true;
}
