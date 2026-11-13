#include "checkoutView.h"
#include "ui_checkoutView.h"

#include "menuItem.h"
#include "viewFormatting.h"

#include <QHeaderView>
#include <QSignalBlocker>
#include <QTableWidgetItem>

namespace {
  enum CartColumn {
    ItemColumn = 0,
    QuantityColumn,
    UnitPriceColumn,
    LineTotalColumn
  };
  const int UnitPriceCentsRole = Qt::UserRole;
}

CheckoutView::CheckoutView(QWidget* parent)
  : QWidget(parent), ui(new Ui::CheckoutView) {
  ui->setupUi(this);
  ui->cartTable->horizontalHeader()->setSectionResizeMode(ItemColumn, QHeaderView::Stretch);
  ui->pointsSpinBox->setSuffix(tr(" pts"));
  clear();
}

CheckoutView::~CheckoutView() {
  delete ui;
}

void CheckoutView::addItem(const std::shared_ptr<MenuItem>& item, int quantity) {
  if (item == nullptr || quantity <= 0) {
    return;
  }

  ui->messageLabel->clear();

  // increase the quantity if the item is already in the cart
  for (std::size_t i = 0; i < cartLines.size(); ++i) {
    if (cartLines[i].first == item->getId()) {
      cartLines[i].second += quantity;
      updateRow(static_cast<int>(i));
      emit checkoutChanged();
      return;
    }
  }

  cartLines.push_back(std::make_pair(item->getId(), quantity));

  const int row = ui->cartTable->rowCount();
  ui->cartTable->insertRow(row);

  QTableWidgetItem* unitPriceCell = new QTableWidgetItem(ViewFormatting::money(item->getPriceCents()));
  unitPriceCell->setData(UnitPriceCentsRole, item->getPriceCents());

  ui->cartTable->setItem(row, ItemColumn, new QTableWidgetItem(item->getName()));
  ui->cartTable->setItem(row, QuantityColumn, new QTableWidgetItem());
  ui->cartTable->setItem(row, UnitPriceColumn, unitPriceCell);
  ui->cartTable->setItem(row, LineTotalColumn, new QTableWidgetItem());
  updateRow(row);

  emit checkoutChanged();
}

void CheckoutView::updateRow(int row) {
  const int quantity = cartLines[static_cast<std::size_t>(row)].second;
  const int unitPrice = ui->cartTable->item(row, UnitPriceColumn)->data(UnitPriceCentsRole).toInt();

  ui->cartTable->item(row, QuantityColumn)->setText(QString::number(quantity));
  ui->cartTable->item(row, LineTotalColumn)->setText(ViewFormatting::money(unitPrice * quantity));
}

std::vector<std::pair<int, int>> CheckoutView::getCartLines() const {
  return cartLines;
}

int CheckoutView::getCartItemCount() const {
  int count = 0;
  for (const std::pair<int, int>& line : cartLines) {
    count += line.second;
  }
  return count;
}

int CheckoutView::getRequestedLoyaltyPoints() const {
  return ui->pointsSpinBox->value();
}

int CheckoutView::getRequestedAccountCreditCents() const {
  return qRound(ui->creditSpinBox->value() * 100.0);
}

void CheckoutView::setAvailableBalances(int loyaltyPoints, int accountCreditCents) {
  // limit requests to the available balances
  // the pricing model still validates the final redemption
  availableLoyaltyPoints = qMax(0, loyaltyPoints);
  ui->pointsSpinBox->setMaximum(availableLoyaltyPoints);
  ui->creditSpinBox->setMaximum(qMax(0, accountCreditCents) / 100.0);

  ui->availablePointsLabel->setText(tr("Available: %1 %2")
    .arg(loyaltyPoints)
    .arg(loyaltyPoints == 1 ? tr("point") : tr("points")));
  ui->availableCreditLabel->setText(tr("Available: %1").arg(ViewFormatting::money(accountCreditCents)));
}

void CheckoutView::showPriceBreakdown(const PriceBreakdown& price) {
  using ViewFormatting::money;

  ui->subtotalValueLabel->setText(money(price.subtotalCents));
  ui->taxValueLabel->setText(money(price.taxCents));
  ui->totalValueLabel->setText(money(price.totalCents));

  // limit the points input to 50 percent of the subtotal and the available balance
  const int maximumPoints = qMin(availableLoyaltyPoints, price.subtotalCents / 200);
  const int previousPoints = ui->pointsSpinBox->value();
  {
    QSignalBlocker blocker(ui->pointsSpinBox);
    ui->pointsSpinBox->setMaximum(qMax(0, maximumPoints));
  }

  // refresh the price if the new limit reduced the entered points
  if (ui->pointsSpinBox->value() != previousPoints) {
    emit checkoutChanged();
    return;
  }

  const QString pointsUnit = price.loyaltyPointsUsed == 1 ? tr("pt") : tr("pts");
  const QString pointsDiscount = price.loyaltyValueCents > 0
    ? QStringLiteral("-") + money(price.loyaltyValueCents)
    : money(0);

  ui->pointsRedeemedValueLabel->setText(
    tr("%1 %2 (%3)").arg(price.loyaltyPointsUsed).arg(pointsUnit).arg(pointsDiscount));
  ui->creditAppliedValueLabel->setText(
    price.accountCreditUsedCents > 0
      ? QStringLiteral("-") + money(price.accountCreditUsedCents)
      : money(0));
  ui->amountDueValueLabel->setText(money(price.amountChargedCents));
  ui->pointsEarnedValueLabel->setText(tr("%1 points").arg(price.loyaltyPointsEarned));

  // explain when the model limits the requested points
  if (ui->pointsSpinBox->value() > price.loyaltyPointsUsed && !cartLines.empty()) {
    ui->pointsNoteLabel->setText(
      tr("Only %1 points can be used on this order.").arg(price.loyaltyPointsUsed));
  } else {
    ui->pointsNoteLabel->clear();
  }

  ui->placeOrderButton->setEnabled(!cartLines.empty());
}

void CheckoutView::showMessage(const QString& message, bool isError) {
  ui->messageLabel->setStyleSheet(isError ? QStringLiteral("color: #c62828;")
                      : QStringLiteral("color: #2e7d32;"));
  ui->messageLabel->setText(message);
}

void CheckoutView::clearCart() {
  {
    // suppress extra signals while resetting the redemption inputs
    QSignalBlocker pointsBlocker(ui->pointsSpinBox);
    QSignalBlocker creditBlocker(ui->creditSpinBox);
    ui->pointsSpinBox->setValue(0);
    ui->creditSpinBox->setValue(0.0);
  }

  cartLines.clear();
  ui->cartTable->setRowCount(0);

  emit checkoutChanged();
}

void CheckoutView::clear() {
  clearCart();
  ui->messageLabel->clear();
  ui->pointsNoteLabel->clear();
  setAvailableBalances(0, 0);
  showPriceBreakdown(PriceBreakdown());
}

void CheckoutView::on_removeItemButton_clicked() {
  const int row = ui->cartTable->currentRow();

  if (row < 0 || row >= static_cast<int>(cartLines.size())) {
    showMessage(tr("Select an item in the cart to remove."), true);
    return;
  }

  cartLines.erase(cartLines.begin() + row);
  ui->cartTable->removeRow(row);
  ui->messageLabel->clear();

  emit checkoutChanged();
}

void CheckoutView::on_clearCartButton_clicked() {
  ui->messageLabel->clear();
  clearCart();
}

void CheckoutView::on_pointsSpinBox_valueChanged(int points) {
  ui->pointsSpinBox->setSuffix(points == 1 ? tr(" pt") : tr(" pts"));
  emit checkoutChanged();
}

void CheckoutView::on_creditSpinBox_valueChanged(double) {
  emit checkoutChanged();
}

void CheckoutView::on_placeOrderButton_clicked() {
  emit placeOrderRequested();
}