#include "kitchenWorkspace.h"
#include "ui_kitchenWorkspace.h"

#include "kitchenController.h"

#include "kitchenQueueView.h"
#include "orderDetailsView.h"

#include <memory>
#include <vector>

KitchenWorkspace::KitchenWorkspace(KitchenController& kitchenController, QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::KitchenWorkspace)
    , kitchenController(kitchenController)
    , queueView(nullptr)
    , detailsView(nullptr)
    , selectedOrderNumber(-1)
{
    ui->setupUi(this);

    queueView = new KitchenQueueView(this);
    detailsView = new OrderDetailsView(this);

    // queue on the left, details of the selected order on the right
    ui->contentSplitter->addWidget(queueView);
    ui->contentSplitter->addWidget(detailsView);
    ui->contentSplitter->setStretchFactor(0, 1);
    ui->contentSplitter->setStretchFactor(1, 1);

    connect(queueView, &KitchenQueueView::orderSelected,
            this, &KitchenWorkspace::handleOrderSelected);
    connect(detailsView, &OrderDetailsView::completeRequested,
            this, &KitchenWorkspace::handleCompleteRequested);
}

KitchenWorkspace::~KitchenWorkspace()
{
    delete ui;
}

void KitchenWorkspace::loadForUser(const QString& staffName)
{
    clear();
    ui->welcomeLabel->setText(tr("Kitchen: %1").arg(staffName));
    refreshQueue();
}

void KitchenWorkspace::clear()
{
    selectedOrderNumber = -1;
    ui->welcomeLabel->clear();
    queueView->clear();
    detailsView->clear();
}

void KitchenWorkspace::on_logoutButton_clicked()
{
    emit logoutRequested();
}

void KitchenWorkspace::on_refreshButton_clicked()
{
    refreshQueue();
}

void KitchenWorkspace::refreshQueue()
{
    std::vector<std::shared_ptr<Order>> orders = kitchenController.getQueuedOrders();
    queueView->showOrders(orders);

    if (orders.empty()) {
        selectedOrderNumber = -1;
        detailsView->clear();
        return;
    }

    // keep the current selection if that order is still queued, otherwise show the next order
    if (selectedOrderNumber == -1 || !queueView->selectOrder(selectedOrderNumber)) {
        queueView->selectOrder(orders.front()->getOrderNumber());
    }
}

void KitchenWorkspace::handleOrderSelected(int orderNumber)
{
    std::shared_ptr<Order> order = kitchenController.getOrderDetails(orderNumber);

    if (order == nullptr) {
        selectedOrderNumber = -1;
        detailsView->clear();
        return;
    }

    selectedOrderNumber = orderNumber;

    // any queued order can be viewed; only the front one can be completed
    // (KitchenController::completeOrder also enforces this)
    std::vector<std::shared_ptr<Order>> orders = kitchenController.getQueuedOrders();
    const bool isFrontOfQueue = !orders.empty() && orders.front()->getOrderNumber() == orderNumber;

    detailsView->showOrder(order, isFrontOfQueue);
}

void KitchenWorkspace::handleCompleteRequested(int orderNumber)
{
    KitchenActionResult result = kitchenController.completeOrder(orderNumber);

    if (result.success) {
        selectedOrderNumber = -1; // move on to the next order in the queue
    }

    refreshQueue();

    QString message = result.message;
    if (result.success) {
        message = tr("Order #%1 is completed and ready for pick up.").arg(orderNumber);
    }
    detailsView->showMessage(message, !result.success);
}
