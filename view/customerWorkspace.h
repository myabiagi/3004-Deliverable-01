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
namespace Ui { class CustomerWorkspace; }
QT_END_NAMESPACE

// Customer screen. Owns the Menu, Cart/Checkout and Dashboard tabs and mediates
// between them: child views only emit signals, and this workspace is the only
// customer view that talks to CustomerController and OrderController.
class CustomerWorkspace : public QWidget
{
    Q_OBJECT

public:
    CustomerWorkspace(CustomerController& customerController,
                      OrderController& orderController,
                      QWidget *parent = nullptr);
    ~CustomerWorkspace();

    // load menu, balances and dashboard for the customer who just logged in
    void loadForCurrentUser();

    // remove everything the previous customer could see (cart included)
    void clear();

signals:
    void logoutRequested();

private slots:
    void on_logoutButton_clicked();
    void on_tabWidget_currentChanged(int index);

    void handleAddToCart(int menuItemId, int quantity);
    void refreshCheckoutPreview();
    void handlePlaceOrder();
    void handleEmailUpdate(const QString& email);
    void handleHomeAddressUpdate(const QString& homeAddress);

private:
    void loadMenu();
    void refreshCustomerData();
    void updateCartTabTitle();

    // translate the cart's (menu item id, quantity) pairs into OrderItems
    std::vector<OrderItem> buildOrderItems() const;
    std::shared_ptr<MenuItem> findMenuItem(int menuItemId) const;

    Ui::CustomerWorkspace *ui;
    CustomerController& customerController;
    OrderController& orderController;

    MenuView* menuView;
    CheckoutView* checkoutView;
    DashboardView* dashboardView;
};

#endif // CUSTOMERWORKSPACE_H
