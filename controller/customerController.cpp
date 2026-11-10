#include "customercontroller.h"

CustomerController::CustomerController(
    SessionController& sessionController,
    MenuCatalog& menuCatalog)
    : sessionController(sessionController),
      menuCatalog(menuCatalog)
{
}

std::shared_ptr<Customer>
CustomerController::getCurrentCustomer() const
{
    std::shared_ptr<User> currentUser =
        sessionController.getCurrentUser();

    if (currentUser == nullptr)
    {
        return nullptr;
    }

    // return nullptr if the active user is not a customer
    return std::dynamic_pointer_cast<Customer>(currentUser);
}

const std::vector<std::shared_ptr<MenuItem>>&
CustomerController::getMenuItems() const
{
    return menuCatalog.getAllItems();
}

std::vector<std::shared_ptr<MenuItem>>
CustomerController::getMenuItemsByCategory(
    MenuCategory category) const
{
    return menuCatalog.getItemsByCategory(category);
}

std::shared_ptr<Order>
CustomerController::getCurrentOrder() const
{
    std::shared_ptr<Customer> customer =
        getCurrentCustomer();

    if (customer == nullptr)
    {
        return nullptr;
    }

    return customer->getCurrentOrder();
}

std::vector<std::shared_ptr<Order>>
CustomerController::getOrderHistory() const
{
    std::shared_ptr<Customer> customer =
        getCurrentCustomer();

    if (customer == nullptr)
    {
        return {};
    }

    return customer->getOrderHistory();
}
