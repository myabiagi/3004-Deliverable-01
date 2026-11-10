#ifndef ORDERITEM_H
#define ORDERITEM_H

#include <QtGlobal>
#include <memory>

#include "menuitem.h"

class OrderItem
{
public:
    OrderItem(
        const std::shared_ptr<MenuItem>& menuItem,
        int quantity);

    std::shared_ptr<MenuItem> getMenuItem() const;
    int getQuantity() const;
    qint64 getUnitPriceCents() const;
    qint64 getTotalPriceCents() const;

private:
    std::shared_ptr<MenuItem> menuItem;
    int quantity;

    // stores the item's price at the time the order is placed
    qint64 unitPriceCents;
};

#endif 
