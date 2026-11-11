#ifndef CUSTOMERCONTROLLER_H
#define CUSTOMERCONTROLLER_H

#include "sessionController.h"
#include "customer.h"
#include "menuCatalog.h"
#include "menuItem.h"
#include "order.h"
#include "orderRepository.h"

#include <QString>
#include <memory>
#include <vector>

struct ProfileUpdateResult {
    bool success = false;
    QString message;
};

// Provides customer-facing info like current account, cafe menu, current and historical orders placed by the customer user
class CustomerController {
public:
    CustomerController(SessionController& sessionController,
                       MenuCatalog& menuCatalog,
                       OrderRepository& orderRepository);

    // return the active user as a customer
    std::shared_ptr<Customer> getCurrentCustomer() const;

    // return all menu items
    const std::vector<std::shared_ptr<MenuItem>>& getMenuItems() const;

    // return menu items belonging to one category
    std::vector<std::shared_ptr<MenuItem>> getMenuItemsByCategory(MenuCategory category) const;

    // return the customer's current in-progress or completed order
    std::shared_ptr<Order> getCurrentOrder() const;

    // return the customer's picked-up and cancelled order history
    std::vector<std::shared_ptr<Order>> getOrderHistory() const;

    // update the logged-in customer's editable dashboard fields
    ProfileUpdateResult updateEmail(const QString& email);
    ProfileUpdateResult updateHomeAddress(const QString& homeAddress);

private:
    SessionController& sessionController;
    MenuCatalog& menuCatalog;
    OrderRepository& orderRepository;
};

#endif
