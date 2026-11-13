#ifndef CUSTOMERWORKSPACE_H
#define CUSTOMERWORKSPACE_H

#include <QWidget>
#include <QString>

#include <memory>
#include <vector>

class CustomerController;
class OrderController;
class MenuItem;
class OrderItem;

class MenuView;
class CheckoutView;
class DashboardView;

QT_BEGIN_NAMESPACE
namespace Ui {
  class CustomerWorkspace;
}
QT_END_NAMESPACE

// coordinates the customer's menu, checkout and dashboard views
class CustomerWorkspace : public QWidget {
  Q_OBJECT

public:
  CustomerWorkspace(CustomerController& customerController, OrderController& orderController, QWidget* parent = nullptr);
  ~CustomerWorkspace();

  // load information for the current customer
  void loadForCurrentUser();

  // clear the previous customer's information and temporary cart
  void clear();

signals:
  void logoutRequested();

private slots:
  void on_logoutButton_clicked();
  void on_tabWidget_currentChanged(int index);

  // respond to requests from the child views
  void handleAddToCart(int menuItemId, int quantity);
  void refreshCheckoutPreview();
  void handlePlaceOrder();
  void handleEmailUpdate(const QString& email);
  void handleHomeAddressUpdate(const QString& homeAddress);

private:
  void loadMenu();
  void refreshCustomerData();
  void updateCartTabTitle();

  // convert cart entries into order items for the controller
  std::vector<OrderItem> buildOrderItems() const;
  std::shared_ptr<MenuItem> findMenuItem(int menuItemId) const;

  Ui::CustomerWorkspace* ui;
  CustomerController& customerController;
  OrderController& orderController;

  MenuView* menuView;
  CheckoutView* checkoutView;
  DashboardView* dashboardView;
};

#endif
