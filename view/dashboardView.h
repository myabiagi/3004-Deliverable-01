#ifndef DASHBOARDVIEW_H
#define DASHBOARDVIEW_H

#include <QWidget>
#include <QString>

#include <memory>
#include <vector>

class Customer;
class Order;

QT_BEGIN_NAMESPACE
namespace Ui { class DashboardView; }
QT_END_NAMESPACE

// Customer online dashboard: account details and balances, the current order
// (In Progress or Completed) and the five most recent Picked Up / Cancelled orders.
// Renders what CustomerWorkspace passes in; it does not keep Model objects.
class DashboardView : public QWidget
{
    Q_OBJECT

public:
    explicit DashboardView(QWidget *parent = nullptr);
    ~DashboardView();

    void showCustomer(const std::shared_ptr<Customer>& customer);
    void showCurrentOrder(const std::shared_ptr<Order>& order);
    void showOrderHistory(const std::vector<std::shared_ptr<Order>>& orders);
    void showProfileMessage(const QString& message, bool isError);
    void clear();

signals:
    void updateEmailRequested(const QString& email);
    void updateHomeAddressRequested(const QString& homeAddress);

private slots:
    void on_saveEmailButton_clicked();
    void on_saveAddressButton_clicked();

private:
    Ui::DashboardView *ui;
};

#endif // DASHBOARDVIEW_H
