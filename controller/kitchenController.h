#ifndef KITCHENCONTROLLER_H
#define KITCHENCONTROLLER_H

#include "userdirectory.h"
#include "orderqueue.h"

#include <QString>
#include <memory>
#include <vector>

struct KitchenActionResult
{
    bool success = false;
    QString message;
};

class KitchenController
{
public:
    KitchenController(UserDirectory& userDirectory,
                      OrderQueue& orderQueue);

    // return queued orders in fifo order
    std::vector<std::shared_ptr<Order>>
    getQueuedOrders() const;

    // return one queued order
    std::shared_ptr<Order>
    getOrderDetails(int orderNumber) const;

    // change an order from in progress to completed
    KitchenActionResult completeOrder(int orderNumber);

private:
    UserDirectory& userDirectory;
    OrderQueue& orderQueue;
};

#endif
