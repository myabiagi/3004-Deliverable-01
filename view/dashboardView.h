#ifndef DASHBOARDVIEW_H
#define DASHBOARDVIEW_H

#include <QWidget>
#include <QString>

#include <memory>
#include <vector>

class Customer;
class Order;

QT_BEGIN_NAMESPACE
namespace Ui {
  class DashboardView;
}
QT_END_NAMESPACE

// displays customer information, current orders and order history
class DashboardView : public QWidget {
  Q_OBJECT

public:
  explicit DashboardView(QWidget* parent = nullptr);
  ~DashboardView();

  // display the customer's account information
  void showCustomer(const std::shared_ptr<Customer>& customer);

  // display the current order and five most recent historical orders
  void showCurrentOrder(const std::shared_ptr<Order>& order);
  void showOrderHistory(const std::vector<std::shared_ptr<Order>>& orders);

  // display profile update feedback or reset the dashboard
  void showProfileMessage(const QString& message, bool isError);
  void clear();

signals:
  // request updates to the customer's contact information
  void updateEmailRequested(const QString& email);
  void updateHomeAddressRequested(const QString& homeAddress);

private slots:
  void on_saveEmailButton_clicked();
  void on_saveAddressButton_clicked();

private:
  Ui::DashboardView* ui;
};

#endif
