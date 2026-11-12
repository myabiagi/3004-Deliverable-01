#include "dashboardView.h"
#include "ui_dashboardView.h"

#include "customer.h"
#include "order.h"
#include "viewFormatting.h"

#include <QHeaderView>
#include <QTableWidgetItem>

namespace {
enum HistoryColumn {
    OrderNumberColumn = 0,
    DateColumn,
    TypeColumn,
    ItemsColumn,
    FinalPriceColumn,
    AmountChargedColumn,
    PointsUsedColumn,
    CreditUsedColumn,
    PointsEarnedColumn,
    StatusColumn
};
}

DashboardView::DashboardView(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::DashboardView)
{
    ui->setupUi(this);
    ui->historyTable->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    ui->historyTable->horizontalHeader()->setSectionResizeMode(ItemsColumn, QHeaderView::Stretch);
    clear();
}

DashboardView::~DashboardView()
{
    delete ui;
}

void DashboardView::showCustomer(const std::shared_ptr<Customer>& customer)
{
    if (customer == nullptr) {
        clear();
        return;
    }

    ui->nameValueLabel->setText(customer->getName());
    ui->usernameValueLabel->setText(customer->getUsername());
    ui->loyaltyNumberValueLabel->setText(customer->getLoyaltyNumber());
    ui->emailLineEdit->setText(customer->getEmail());
    ui->addressLineEdit->setText(customer->getHomeAddress());
    ui->pointsBalanceValueLabel->setText(tr("%1 points").arg(customer->getLoyaltyPoints()));
    ui->creditBalanceValueLabel->setText(ViewFormatting::money(customer->getAccountCreditCents()));
}

void DashboardView::showCurrentOrder(const std::shared_ptr<Order>& order)
{
    using ViewFormatting::money;

    if (order == nullptr) {
        ui->currentOrderLabel->setText(tr("You have no current order."));
        return;
    }

    QString pointsEarned = tr("%1 points").arg(order->getLoyaltyPointsEarned());
    if (order->getStatus() == OrderStatus::InProgress && order->getLoyaltyPointsEarned() > 0) {
        pointsEarned += tr(" (added when the order is completed)");
    }

    const QString html = tr(
        "<p style='font-size:15px'><b>Order #%1</b> &mdash; <b>%2</b></p>"
        "<table cellspacing='4'>"
        "<tr><td>Placed:</td><td>%3 (%4)</td></tr>"
        "<tr><td>Items:</td><td>%5</td></tr>"
        "<tr><td>Final price (incl. tax):</td><td>%6</td></tr>"
        "<tr><td>Loyalty points used:</td><td>%7</td></tr>"
        "<tr><td>Account credit used:</td><td>%8</td></tr>"
        "<tr><td>Amount charged:</td><td>%9</td></tr>"
        "<tr><td>Points earned:</td><td>%10</td></tr>"
        "</table>")
        .arg(order->getOrderNumber())
        .arg(orderStatusToString(order->getStatus()).toHtmlEscaped())
        .arg(order->getPlacedAtDisplayString().toHtmlEscaped())
        .arg(orderSourceToString(order->getSource()).toHtmlEscaped())
        .arg(ViewFormatting::orderItemsSummary(*order).toHtmlEscaped())
        .arg(money(order->getTotalCents()))
        .arg(order->getLoyaltyPointsUsed())
        .arg(money(order->getAccountCreditUsedCents()))
        .arg(money(order->getAmountChargedCents()))
        .arg(pointsEarned.toHtmlEscaped());

    ui->currentOrderLabel->setText(html);
}

void DashboardView::showOrderHistory(const std::vector<std::shared_ptr<Order>>& orders)
{
    using ViewFormatting::money;

    ui->historyTable->setRowCount(0);

    for (const std::shared_ptr<Order>& order : orders) {
        if (order == nullptr) {
            continue;
        }

        const int row = ui->historyTable->rowCount();
        ui->historyTable->insertRow(row);

        ui->historyTable->setItem(row, OrderNumberColumn,
                                  new QTableWidgetItem(QString::number(order->getOrderNumber())));
        ui->historyTable->setItem(row, DateColumn,
                                  new QTableWidgetItem(order->getPlacedAtDisplayString()));
        ui->historyTable->setItem(row, TypeColumn,
                                  new QTableWidgetItem(orderSourceToString(order->getSource())));
        ui->historyTable->setItem(row, ItemsColumn,
                                  new QTableWidgetItem(ViewFormatting::orderItemsSummary(*order)));
        ui->historyTable->setItem(row, FinalPriceColumn,
                                  new QTableWidgetItem(money(order->getTotalCents())));
        ui->historyTable->setItem(row, AmountChargedColumn,
                                  new QTableWidgetItem(money(order->getAmountChargedCents())));
        ui->historyTable->setItem(row, PointsUsedColumn,
                                  new QTableWidgetItem(QString::number(order->getLoyaltyPointsUsed())));
        ui->historyTable->setItem(row, CreditUsedColumn,
                                  new QTableWidgetItem(money(order->getAccountCreditUsedCents())));
        ui->historyTable->setItem(row, PointsEarnedColumn,
                                  new QTableWidgetItem(QString::number(order->getLoyaltyPointsEarned())));
        ui->historyTable->setItem(row, StatusColumn,
                                  new QTableWidgetItem(orderStatusToString(order->getStatus())));
    }
}

void DashboardView::showProfileMessage(const QString& message, bool isError)
{
    ui->profileMessageLabel->setStyleSheet(isError ? QStringLiteral("color: #c62828;")
                                                   : QStringLiteral("color: #2e7d32;"));
    ui->profileMessageLabel->setText(message);
}

void DashboardView::clear()
{
    ui->nameValueLabel->clear();
    ui->usernameValueLabel->clear();
    ui->loyaltyNumberValueLabel->clear();
    ui->emailLineEdit->clear();
    ui->addressLineEdit->clear();
    ui->pointsBalanceValueLabel->clear();
    ui->creditBalanceValueLabel->clear();
    ui->profileMessageLabel->clear();
    ui->currentOrderLabel->clear();
    ui->historyTable->setRowCount(0);
}

void DashboardView::on_saveEmailButton_clicked()
{
    emit updateEmailRequested(ui->emailLineEdit->text());
}

void DashboardView::on_saveAddressButton_clicked()
{
    emit updateHomeAddressRequested(ui->addressLineEdit->text());
}
