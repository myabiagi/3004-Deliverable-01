#include "orderqueue.h"

bool OrderQueue::enqueue(const std::shared_ptr<Order>& order)
{
    if (order == nullptr)
    {
        return false;
    }

    // only paid, in-progress orders belong in the kitchen queue
    if (order->getStatus() != OrderStatus::InProgress)
    {
        return false;
    }

    // prevent the same order from being added more than once
    if (findByOrderNumber(order->getOrderNumber()) != nullptr)
    {
        return false;
    }

    orders.push_back(order);
    return true;
}

std::shared_ptr<Order> OrderQueue::front() const
{
    if (orders.empty())
    {
        return nullptr;
    }

    return orders.front();
}

std::shared_ptr<Order>
OrderQueue::findByOrderNumber(int orderNumber) const
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

bool OrderQueue::removeByOrderNumber(int orderNumber)
{
    for (auto iterator = orders.begin();
         iterator != orders.end();
         ++iterator)
    {
        if ((*iterator)->getOrderNumber() == orderNumber)
        {
            orders.erase(iterator);
            return true;
        }
    }

    return false;
}

const std::deque<std::shared_ptr<Order>>&
OrderQueue::getOrders() const
{
    return orders;
}

bool OrderQueue::isEmpty() const
{
    return orders.empty();
}

std::size_t OrderQueue::size() const
{
    return orders.size();
}
