#include "mainwindow.h"

#include "applicationController.h"
#include "customerController.h"
#include "kitchenController.h"
#include "orderController.h"
#include "sessionController.h"

#include "defaultDataLoader.h"
#include "menuCatalog.h"
#include "orderQueue.h"
#include "orderRepository.h"
#include "pricingService.h"
#include "userDirectory.h"

#include <QApplication>
#include <QDebug>

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);

    // create the shared model objects
    UserDirectory users;
    MenuCatalog menu;
    OrderRepository orders;
    OrderQueue orderQueue(orders);
    PricingService pricingService;

    // create all default users, menu items, and historical orders
    DefaultDataLoader dataLoader;

    if (!dataLoader.loadDefaults(users, menu, orders)) {
        qCritical() << "Failed to load default system data";
        return 1;
    }

    // create controllers using the shared model objects
    SessionController sessionController(users);
    ApplicationController applicationController(sessionController);
    CustomerController customerController(sessionController, menu, orders);
    OrderController orderController(sessionController, orders, pricingService);
    KitchenController kitchenController(orderQueue);

    // Views are not wired yet; keep the non-view stack alive for D1 domain use.
    Q_UNUSED(applicationController);
    Q_UNUSED(customerController);
    Q_UNUSED(orderController);
    Q_UNUSED(kitchenController);

    // display the main application window
    MainWindow w;
    w.show();

    return a.exec();
}
