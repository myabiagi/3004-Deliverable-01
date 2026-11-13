#include "kitchenQueueView.h"
#include "ui_kitchenQueueView.h"

#include "order.h"

#include <QHeaderView>
#include <QSignalBlocker>
#include <QTableWidgetItem>

namespace {
  enum QueueColumn {
    PositionColumn = 0,
    OrderNumberColumn,
    CustomerColumn,
    PlacedColumn
  };

  const int OrderNumberRole = Qt::UserRole;
}

KitchenQueueView::KitchenQueueView(QWidget* parent)
  : QWidget(parent), ui(new Ui::KitchenQueueView) {
  ui->setupUi(this);
  ui->queueTable->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
  ui->queueTable->horizontalHeader()->setSectionResizeMode(CustomerColumn, QHeaderView::Stretch);
  clear();
}

KitchenQueueView::~KitchenQueueView() {
  delete ui;
}

void KitchenQueueView::showOrders(const std::vector<std::shared_ptr<Order>>& orders) {
  // rebuild the table without emitting selection signals for each row
  QSignalBlocker blocker(ui->queueTable);
  ui->queueTable->setRowCount(0);

  int position = 1;

  for (const std::shared_ptr<Order>& order : orders) {
    if (order == nullptr) {
      continue;
    }

    const int row = ui->queueTable->rowCount();
    ui->queueTable->insertRow(row);

    QTableWidgetItem* positionItem = new QTableWidgetItem(
      position == 1 ? tr("1 (next)") : QString::number(position)
    );

    positionItem->setData(OrderNumberRole, order->getOrderNumber());

    ui->queueTable->setItem(row, PositionColumn, positionItem);

    ui->queueTable->setItem(row, OrderNumberColumn,
      new QTableWidgetItem(QString::number(order->getOrderNumber())));

    ui->queueTable->setItem(row, CustomerColumn,
      new QTableWidgetItem(order->getCustomerName()));

    ui->queueTable->setItem(row, PlacedColumn,
      new QTableWidgetItem(order->getPlacedAtDisplayString()));

    ++position;
  }

  const int count = ui->queueTable->rowCount();

  ui->countLabel->setText(
    count == 0 ? tr("No orders in the queue.")
               : tr("%1 order(s) in the queue.").arg(count)
  );
}

bool KitchenQueueView::selectOrder(int orderNumber) {
  for (int row = 0; row < ui->queueTable->rowCount(); ++row) {
    if (ui->queueTable->item(row, PositionColumn)->data(OrderNumberRole).toInt() == orderNumber) {
      ui->queueTable->clearSelection();

      // selecting a row emits a signal with its order number
      ui->queueTable->selectRow(row);
      return true;
    }
  }

  return false;
}

void KitchenQueueView::clear() {
  QSignalBlocker blocker(ui->queueTable);
  ui->queueTable->setRowCount(0);
  ui->countLabel->setText(tr("No orders in the queue."));
}

void KitchenQueueView::on_queueTable_itemSelectionChanged() {
  const QList<QTableWidgetItem*> selected = ui->queueTable->selectedItems();

  if (selected.isEmpty()) {
    return;
  }

  const int row = selected.first()->row();

  emit orderSelected(
    ui->queueTable->item(row, PositionColumn)->data(OrderNumberRole).toInt()
  );
}
