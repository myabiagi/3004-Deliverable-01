#include "orderitem.h"

OrderItem::OrderItem(
    const std::shared_ptr<MenuItem>& menuItem,
    int quantity)
    : menuItem(menuItem),
      quantity(quantity),
      unitPriceCents(menuItem->getPriceCents())
{
}

std::shared_ptr<MenuItem> OrderItem::getMenuItem() const
{
    return menuItem;
}

int OrderItem::getQuantity() const
{
    return quantity;
}

qint64 OrderItem::getUnitPriceCents() const
{
    return unitPriceCents;
}

qint64 OrderItem::getTotalPriceCents() const
{
    return unitPriceCents * quantity;
}
