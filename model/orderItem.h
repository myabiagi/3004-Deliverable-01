#ifndef ORDERITEM_H
#define ORDERITEM_H

#include <QtGlobal>
#include <memory>
#include "menuItem.h"

class OrderItem {
public:
    OrderItem(const std::shared_ptr<MenuItem>& menuItem, int quantity);
    std::shared_ptr<MenuItem> getMenuItem() const;
    int getQuantity() const;
    qint64 getUnitPriceCents() const;
    qint64 getTotalPriceCents() const;

private:
    std::shared_ptr<MenuItem> menuItem;
    int quantity;
    qint64 unitPriceCents;
};

#endif