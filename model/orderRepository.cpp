#include "orderrepository.h"

OrderRepository::OrderRepository()
    : nextOrderNumber(1)
{
}

bool OrderRepository::addOrder(
    const std::shared_ptr<Order>& order)
{
    if (order == nullptr)
    {
        return false;
    }

    // prevent two orders from using the same order number
    if (findByOrderNumber(order->getOrderNumber()) != nullptr)
    {
        return false;
    }

    orders.push_back(order);

    // keep future generated numbers above manually loaded order numbers
    if (order->getOrderNumber() >= nextOrderNumber)
    {
        nextOrderNumber = order->getOrderNumber() + 1;
    }

    return true;
}

std::shared_ptr<Order>
OrderRepository::findByOrderNumber(int orderNumber) const
{
    for (const std::shared_ptr<Order>& order : orders)
    {
        if (order->getOrderNumber() == orderNumber)
        {
            return order;
        }
    }

    return nullptr;
}

int OrderRepository::generateOrderNumber()
{
    int generatedNumber = nextOrderNumber;
    nextOrderNumber++;

    return generatedNumber;
}

const std::vector<std::shared_ptr<Order>>&
OrderRepository::getAllOrders() const
{
    return orders;
}
