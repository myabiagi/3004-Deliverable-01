#include "checkoutView.h"
#include "ui_checkoutView.h"

#include "menuItem.h"
#include "viewFormatting.h"

#include <QHeaderView>
#include <QSignalBlocker>
#include <QTableWidgetItem>

namespace {
enum CartColumn { ItemColumn = 0, QuantityColumn, UnitPriceColumn, LineTotalColumn };
const int UnitPriceCentsRole = Qt::UserRole;
}

CheckoutView::CheckoutView(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::CheckoutView)
{
    ui->setupUi(this);
    ui->cartTable->horizontalHeader()->setSectionResizeMode(ItemColumn, QHeaderView::Stretch);
    clear();
}

CheckoutView::~CheckoutView()
{
    delete ui;
}

void CheckoutView::addItem(const std::shared_ptr<MenuItem>& item, int quantity)
{
    if (item == nullptr || quantity <= 0) {
        return;
    }

    ui->messageLabel->clear();

    // same item again: increase the existing line
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

    QTableWidgetItem *unitPriceCell = new QTableWidgetItem(ViewFormatting::money(item->getPriceCents()));
    unitPriceCell->setData(UnitPriceCentsRole, static_cast<qlonglong>(item->getPriceCents()));

    ui->cartTable->setItem(row, ItemColumn, new QTableWidgetItem(item->getName()));
    ui->cartTable->setItem(row, QuantityColumn, new QTableWidgetItem());
    ui->cartTable->setItem(row, UnitPriceColumn, unitPriceCell);
    ui->cartTable->setItem(row, LineTotalColumn, new QTableWidgetItem());
    updateRow(row);

    emit checkoutChanged();
}

void CheckoutView::updateRow(int row)
{
    const int quantity = cartLines[static_cast<std::size_t>(row)].second;
    const qint64 unitPrice = ui->cartTable->item(row, UnitPriceColumn)->data(UnitPriceCentsRole).toLongLong();

    ui->cartTable->item(row, QuantityColumn)->setText(QString::number(quantity));
    ui->cartTable->item(row, LineTotalColumn)->setText(ViewFormatting::money(unitPrice * quantity));
}

std::vector<std::pair<int, int>> CheckoutView::getCartLines() const
{
    return cartLines;
}

int CheckoutView::getCartItemCount() const
{
    int count = 0;
    for (const std::pair<int, int>& line : cartLines) {
        count += line.second;
    }
    return count;
}

int CheckoutView::getRequestedLoyaltyPoints() const
{
    return ui->pointsSpinBox->value();
}

qint64 CheckoutView::getRequestedAccountCreditCents() const
{
    return qRound64(ui->creditSpinBox->value() * 100.0);
}

void CheckoutView::setAvailableBalances(int loyaltyPoints, qint64 accountCreditCents)
{
    // the spin boxes cannot ask for more than the customer has;
    // PricingService still decides how much can actually be applied
    ui->pointsSpinBox->setMaximum(qMax(0, loyaltyPoints));
    ui->creditSpinBox->setMaximum(qMax<qint64>(0, accountCreditCents) / 100.0);

    ui->availablePointsLabel->setText(tr("Available: %1 points").arg(loyaltyPoints));
    ui->availableCreditLabel->setText(tr("Available: %1").arg(ViewFormatting::money(accountCreditCents)));
}

void CheckoutView::showPriceBreakdown(const PriceBreakdown& price)
{
    using ViewFormatting::money;

    ui->subtotalValueLabel->setText(money(price.subtotalCents));
    ui->taxValueLabel->setText(money(price.taxCents));
    ui->totalValueLabel->setText(money(price.totalCents));
    ui->pointsRedeemedValueLabel->setText(
        tr("%1 points (-%2)").arg(price.loyaltyPointsUsed).arg(money(price.loyaltyValueCents)));
    ui->creditAppliedValueLabel->setText(QStringLiteral("-") + money(price.accountCreditUsedCents));
    ui->amountDueValueLabel->setText(money(price.amountChargedCents));
    ui->pointsEarnedValueLabel->setText(tr("%1 points").arg(price.loyaltyPointsEarned));

    // tell the customer when the request was limited by the business rules
    if (ui->pointsSpinBox->value() > price.loyaltyPointsUsed && !cartLines.empty()) {
        ui->pointsNoteLabel->setText(
            tr("Only %1 points can be used on this order.").arg(price.loyaltyPointsUsed));
    } else {
        ui->pointsNoteLabel->clear();
    }

    ui->placeOrderButton->setEnabled(!cartLines.empty());
}

void CheckoutView::showMessage(const QString& message, bool isError)
{
    ui->messageLabel->setStyleSheet(isError ? QStringLiteral("color: #c62828;")
                                            : QStringLiteral("color: #2e7d32;"));
    ui->messageLabel->setText(message);
}

void CheckoutView::clearCart()
{
    {
        // reset quietly, then announce one change
        QSignalBlocker pointsBlocker(ui->pointsSpinBox);
        QSignalBlocker creditBlocker(ui->creditSpinBox);
        ui->pointsSpinBox->setValue(0);
        ui->creditSpinBox->setValue(0.0);
    }

    cartLines.clear();
    ui->cartTable->setRowCount(0);

    emit checkoutChanged();
}

void CheckoutView::clear()
{
    clearCart();
    ui->messageLabel->clear();
    ui->pointsNoteLabel->clear();
    setAvailableBalances(0, 0);
    showPriceBreakdown(PriceBreakdown());
}

void CheckoutView::on_removeItemButton_clicked()
{
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

void CheckoutView::on_clearCartButton_clicked()
{
    ui->messageLabel->clear();
    clearCart();
}

void CheckoutView::on_pointsSpinBox_valueChanged(int value)
{
    Q_UNUSED(value);
    emit checkoutChanged();
}

void CheckoutView::on_creditSpinBox_valueChanged(double value)
{
    Q_UNUSED(value);
    emit checkoutChanged();
}

void CheckoutView::on_placeOrderButton_clicked()
{
    emit placeOrderRequested();
}
