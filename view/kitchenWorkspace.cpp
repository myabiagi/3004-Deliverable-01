#include "kitchenWorkspace.h"
#include "ui_kitchenWorkspace.h"

#include "kitchenController.h"

#include "kitchenQueueView.h"
#include "orderDetailsView.h"

#include <memory>
#include <vector>

KitchenWorkspace::KitchenWorkspace(KitchenController& kitchenController, QWidget* parent)
  : QWidget(parent),
    ui(new Ui::KitchenWorkspace),
    kitchenController(kitchenController),
    queueView(nullptr),
    detailsView(nullptr),
    selectedOrderNumber(-1) {
  ui->setupUi(this);

  // display the queue on the left and selected order details on the right
  queueView = new KitchenQueueView(this);
  detailsView = new OrderDetailsView(this);

  ui->contentSplitter->addWidget(queueView);
  ui->contentSplitter->addWidget(detailsView);
  ui->contentSplitter->setStretchFactor(0, 1);
  ui->contentSplitter->setStretchFactor(1, 1);

  // coordinate requests from the child views
  connect(queueView, &KitchenQueueView::orderSelected,
          this, &KitchenWorkspace::handleOrderSelected);

  connect(detailsView, &OrderDetailsView::completeRequested,
          this, &KitchenWorkspace::handleCompleteRequested);
}

KitchenWorkspace::~KitchenWorkspace() {
  delete ui;
}

void KitchenWorkspace::loadForUser(const QString& staffName) {
  clear();
  ui->welcomeLabel->setText(tr("Kitchen: %1").arg(staffName));
  refreshQueue();
}

void KitchenWorkspace::clear() {
  selectedOrderNumber = -1;
  ui->welcomeLabel->clear();
  queueView->clear();
  detailsView->clear();
}

void KitchenWorkspace::on_logoutButton_clicked() {
  emit logoutRequested();
}

void KitchenWorkspace::on_refreshButton_clicked() {
  refreshQueue();
}

void KitchenWorkspace::refreshQueue() {
  std::vector<std::shared_ptr<Order>> orders = kitchenController.getQueuedOrders();
  queueView->showOrders(orders);

  if (orders.empty()) {
    selectedOrderNumber = -1;
    detailsView->clear();
    return;
  }

  // preserve the selected order or select the front order if needed
  if (selectedOrderNumber == -1 || !queueView->selectOrder(selectedOrderNumber)) {
    queueView->selectOrder(orders.front()->getOrderNumber());
  }
}

void KitchenWorkspace::handleOrderSelected(int orderNumber) {
  std::shared_ptr<Order> order = kitchenController.getOrderDetails(orderNumber);

  if (order == nullptr) {
    selectedOrderNumber = -1;
    detailsView->clear();
    return;
  }

  selectedOrderNumber = orderNumber;

  // any queued order can be viewed but only the front order can be completed
  std::vector<std::shared_ptr<Order>> orders = kitchenController.getQueuedOrders();

  const bool isFrontOfQueue = !orders.empty() &&
    orders.front()->getOrderNumber() == orderNumber;

  detailsView->showOrder(order, isFrontOfQueue);
}

void KitchenWorkspace::handleCompleteRequested(int orderNumber) {
  KitchenActionResult result = kitchenController.completeOrder(orderNumber);

  if (result.success) {
    // select the next queued order after completing the current one
    selectedOrderNumber = -1;
  }

  refreshQueue();

  QString message = result.message;

  if (result.success) {
    message = tr("Order #%1 is completed and ready for pick up.").arg(orderNumber);
  }

  detailsView->showMessage(message, !result.success);
}
