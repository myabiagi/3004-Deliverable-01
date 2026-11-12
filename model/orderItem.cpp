#include "orderItem.h"

OrderItem::OrderItem(const std::shared_ptr<MenuItem>& menuItem, int quantity)
  : menuItem(menuItem),
    quantity(quantity),
    unitPriceCents(menuItem != nullptr ? menuItem->getPriceCents() : 0) {
}

std::shared_ptr<MenuItem> OrderItem::getMenuItem() const { return menuItem; }

int OrderItem::getQuantity() const { return quantity; }

int OrderItem::getUnitPriceCents() const { return unitPriceCents; }

int OrderItem::getTotalPriceCents() const { return unitPriceCents * quantity; }

bool OrderItem::isValid() const {
  // an order item must reference a menu item with a positive quantity and price
  return menuItem != nullptr && quantity > 0 && unitPriceCents > 0;
}
