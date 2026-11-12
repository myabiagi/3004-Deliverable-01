#ifndef CUSTOMERCONTROLLER_H
#define CUSTOMERCONTROLLER_H

#include "sessionController.h"
#include "customer.h"
#include "menuCatalog.h"
#include "order.h"
#include "orderRepository.h"

#include <QString>
#include <memory>
#include <vector>

// result returned after updating customer information
struct ProfileUpdateResult {
  bool success = false;
  QString message;
};

class CustomerController {
public:
  // use the existing session, menu and order storage
  CustomerController(SessionController& sessionController,
                     MenuCatalog& menuCatalog,
                     OrderRepository& orderRepository);

  // retrieve the currently logged-in customer
  std::shared_ptr<Customer> getCurrentCustomer() const;

  // retrieve the cafe menu
  const std::vector<std::shared_ptr<MenuItem>>& getMenuItems() const;
  std::vector<std::shared_ptr<MenuItem>> getMenuItemsByCategory(MenuCategory category) const;

  // retrieve the customer's current and historical orders
  std::shared_ptr<Order> getCurrentOrder() const;
  std::vector<std::shared_ptr<Order>> getOrderHistory() const;

  // request updates to the customer's editable information
  ProfileUpdateResult updateEmail(const QString& email);
  ProfileUpdateResult updateHomeAddress(const QString& homeAddress);

private:
  SessionController& sessionController;
  MenuCatalog& menuCatalog;
  OrderRepository& orderRepository;
};

#endif
