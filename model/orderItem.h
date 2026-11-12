#ifndef ORDERITEM_H
#define ORDERITEM_H

#include "menuItem.h"

#include <memory>

class OrderItem {
public:
  // create a purchased item with its quantity and current unit price
  OrderItem(const std::shared_ptr<MenuItem>& menuItem, int quantity);

  // access purchased item information
  std::shared_ptr<MenuItem> getMenuItem() const;
  int getQuantity() const;
  int getUnitPriceCents() const;
  int getTotalPriceCents() const;

  // check whether the item can be included in an order
  bool isValid() const;

private:
  std::shared_ptr<MenuItem> menuItem;
  int quantity;
  int unitPriceCents;
};

#endif
