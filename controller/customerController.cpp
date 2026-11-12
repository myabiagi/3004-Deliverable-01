#include "customerController.h"

CustomerController::CustomerController(SessionController& sessionController,
                                       MenuCatalog& menuCatalog,
                                       OrderRepository& orderRepository)
  : sessionController(sessionController),
    menuCatalog(menuCatalog),
    orderRepository(orderRepository) {
}

std::shared_ptr<Customer> CustomerController::getCurrentCustomer() const {
  std::shared_ptr<User> currentUser = sessionController.getCurrentUser();

  if (currentUser == nullptr) {
    return nullptr;
  }

  // check whether the active user is a customer
  return std::dynamic_pointer_cast<Customer>(currentUser);
}

const std::vector<std::shared_ptr<MenuItem>>& CustomerController::getMenuItems() const {
  return menuCatalog.getAllItems();
}

std::vector<std::shared_ptr<MenuItem>> CustomerController::getMenuItemsByCategory(MenuCategory category) const {
  return menuCatalog.getItemsByCategory(category);
}

std::shared_ptr<Order> CustomerController::getCurrentOrder() const {
  std::shared_ptr<Customer> customer = getCurrentCustomer();

  if (customer == nullptr) {
    return nullptr;
  }

  return orderRepository.getCurrentOrder(customer->getUsername());
}

std::vector<std::shared_ptr<Order>> CustomerController::getOrderHistory() const {
  std::shared_ptr<Customer> customer = getCurrentCustomer();

  if (customer == nullptr) {
    return {};
  }

  return orderRepository.getOrderHistory(customer->getUsername());
}

ProfileUpdateResult CustomerController::updateEmail(const QString& email) {
  ProfileUpdateResult result;
  std::shared_ptr<Customer> customer = getCurrentCustomer();

  if (customer == nullptr) {
    result.message = "A customer must be logged in.";
    return result;
  }

  // the customer model validates and updates its own email
  if (!customer->setEmail(email)) {
    result.message = "Email address cannot be empty.";
    return result;
  }

  result.success = true;
  result.message = "Email address updated.";
  return result;
}

ProfileUpdateResult CustomerController::updateHomeAddress(const QString& homeAddress) {
  ProfileUpdateResult result;
  std::shared_ptr<Customer> customer = getCurrentCustomer();

  if (customer == nullptr) {
    result.message = "A customer must be logged in.";
    return result;
  }

  // the customer model validates and updates its own home address
  if (!customer->setHomeAddress(homeAddress)) {
    result.message = "Home address cannot be empty.";
    return result;
  }

  result.success = true;
  result.message = "Home address updated.";
  return result;
}
