#include "orderDetailsView.h"
#include "ui_orderDetailsView.h"

#include "order.h"
#include "viewFormatting.h"

#include <QFont>
#include <QHeaderView>
#include <QTreeWidgetItem>

OrderDetailsView::OrderDetailsView(QWidget* parent)
  : QWidget(parent),
    ui(new Ui::OrderDetailsView),
    displayedOrderNumber(-1) {
  ui->setupUi(this);
  ui->itemsTree->header()->setSectionResizeMode(0, QHeaderView::Stretch);
  ui->itemsTree->header()->setSectionResizeMode(1, QHeaderView::ResizeToContents);
  clear();
}

OrderDetailsView::~OrderDetailsView() {
  delete ui;
}

void OrderDetailsView::showOrder(const std::shared_ptr<Order>& order, bool canComplete) {
  if (order == nullptr) {
    clear();
    return;
  }

  displayedOrderNumber = order->getOrderNumber();
  ui->messageLabel->clear();

  ui->orderNumberValueLabel->setText(QString::number(order->getOrderNumber()));
  ui->customerValueLabel->setText(order->getCustomerName());
  ui->placedValueLabel->setText(order->getPlacedAtDisplayString());
  ui->statusValueLabel->setText(orderStatusToString(order->getStatus()));
  ui->typeValueLabel->setText(orderSourceToString(order->getSource()));

  // each purchased item, with the ingredients needed to prepare it
  ui->itemsTree->clear();
  for (const OrderItem& orderItem : order->getItems()) {
    std::shared_ptr<MenuItem> menuItem = orderItem.getMenuItem();
    if (menuItem == nullptr) {
      continue;
    }

    const int quantity = orderItem.getQuantity();

    QTreeWidgetItem* itemRow = new QTreeWidgetItem(ui->itemsTree);
    itemRow->setText(0, menuItem->getName());
    itemRow->setText(1, tr("x %1").arg(quantity));
    QFont boldFont = itemRow->font(0);
    boldFont.setBold(true);
    itemRow->setFont(0, boldFont);
    itemRow->setFont(1, boldFont);

    for (const IngredientRequirement& requirement : menuItem->getIngredientRequirements()) {
      if (requirement.getIngredient() == nullptr) {
        continue;
      }

      const QString unit = requirement.getUnit();
      QString amount = QStringLiteral("%1 %2")
                .arg(ViewFormatting::quantity(requirement.getQuantity() * quantity))
                .arg(unit);
      if (quantity > 1) {
        amount += tr("  (%1 %2 each)")
               .arg(ViewFormatting::quantity(requirement.getQuantity()))
               .arg(unit);
      }

      QTreeWidgetItem* ingredientRow = new QTreeWidgetItem(itemRow);
      ingredientRow->setText(0, requirement.getIngredient()->getName());
      ingredientRow->setText(1, amount.trimmed());
    }

    itemRow->setExpanded(true);
  }

  ui->completeButton->setEnabled(canComplete);
  ui->hintLabel->clear();
}

void OrderDetailsView::showMessage(const QString& message, bool isError) {
  ui->messageLabel->setStyleSheet(isError ? QStringLiteral("color: #c62828;")
                      : QStringLiteral("color: #2e7d32;"));
  ui->messageLabel->setText(message);
}

void OrderDetailsView::clear() {
  displayedOrderNumber = -1;
  ui->orderNumberValueLabel->clear();
  ui->customerValueLabel->clear();
  ui->placedValueLabel->clear();
  ui->statusValueLabel->clear();
  ui->typeValueLabel->clear();
  ui->itemsTree->clear();
  ui->completeButton->setEnabled(false);
  ui->hintLabel->setText(tr("Select an order in the queue to see its details."));
  ui->messageLabel->clear();
}

void OrderDetailsView::on_completeButton_clicked() {
  if (displayedOrderNumber != -1) {
    emit completeRequested(displayedOrderNumber);
  }
}