#include "customerWorkspace.h"
#include "ui_customerWorkspace.h"

#include "customerController.h"
#include "orderController.h"

#include "menuView.h"
#include "checkoutView.h"
#include "dashboardView.h"
#include "viewFormatting.h"

CustomerWorkspace::CustomerWorkspace(CustomerController& customerController,
                                     OrderController& orderController,
                                     QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::CustomerWorkspace)
    , customerController(customerController)
    , orderController(orderController)
    , menuView(nullptr)
    , checkoutView(nullptr)
    , dashboardView(nullptr)
{
    ui->setupUi(this);

    // create all three child views before adding tabs (adding a tab can emit currentChanged)
    menuView = new MenuView(this);
    checkoutView = new CheckoutView(this);
    dashboardView = new DashboardView(this);

    ui->tabWidget->addTab(menuView, tr("Menu"));
    ui->tabWidget->addTab(checkoutView, tr("Cart / Checkout"));
    ui->tabWidget->addTab(dashboardView, tr("My Dashboard"));

    // child views never call controllers directly; the workspace handles their requests
    connect(menuView, &MenuView::addToCartRequested,
            this, &CustomerWorkspace::handleAddToCart);
    connect(checkoutView, &CheckoutView::checkoutChanged,
            this, &CustomerWorkspace::refreshCheckoutPreview);
    connect(checkoutView, &CheckoutView::placeOrderRequested,
            this, &CustomerWorkspace::handlePlaceOrder);
    connect(dashboardView, &DashboardView::updateEmailRequested,
            this, &CustomerWorkspace::handleEmailUpdate);
    connect(dashboardView, &DashboardView::updateHomeAddressRequested,
            this, &CustomerWorkspace::handleHomeAddressUpdate);
}

CustomerWorkspace::~CustomerWorkspace()
{
    delete ui;
}

void CustomerWorkspace::loadForCurrentUser()
{
    clear();
    loadMenu();
    refreshCustomerData();
    refreshCheckoutPreview();

    // the menu is the default tab after logging in
    ui->tabWidget->setCurrentWidget(menuView);
}

void CustomerWorkspace::clear()
{
    ui->welcomeLabel->clear();
    menuView->clearMenu();
    checkoutView->clear();
    dashboardView->clear();
    updateCartTabTitle();
}

void CustomerWorkspace::on_logoutButton_clicked()
{
    emit logoutRequested();
}

void CustomerWorkspace::on_tabWidget_currentChanged(int index)
{
    Q_UNUSED(index);

    // dashboard and balances always show the latest in-memory data
    refreshCustomerData();
}

void CustomerWorkspace::loadMenu()
{
    menuView->clearMenu();

    const MenuCategory categories[] = {
        MenuCategory::Sandwich,
        MenuCategory::HotBeverage,
        MenuCategory::ColdBeverage
    };

    for (MenuCategory category : categories) {
        menuView->addCategory(menuCategoryToString(category),
                              customerController.getMenuItemsByCategory(category));
    }
}

void CustomerWorkspace::refreshCustomerData()
{
    std::shared_ptr<Customer> customer = customerController.getCurrentCustomer();

    if (customer == nullptr) {
        return;
    }

    ui->welcomeLabel->setText(tr("Welcome, %1").arg(customer->getName()));

    checkoutView->setAvailableBalances(customer->getLoyaltyPoints(),
                                       customer->getAccountCreditCents());

    dashboardView->showCustomer(customer);
    dashboardView->showCurrentOrder(customerController.getCurrentOrder());
    dashboardView->showOrderHistory(customerController.getOrderHistory());
}

void CustomerWorkspace::handleAddToCart(int menuItemId, int quantity)
{
    std::shared_ptr<MenuItem> item = findMenuItem(menuItemId);

    if (item == nullptr) {
        return;
    }

    checkoutView->addItem(item, quantity);
}

void CustomerWorkspace::refreshCheckoutPreview()
{
    // OrderController -> PricingService calculates every number shown at checkout
    PriceBreakdown price = orderController.previewOnlineOrder(
        buildOrderItems(),
        checkoutView->getRequestedLoyaltyPoints(),
        checkoutView->getRequestedAccountCreditCents());

    checkoutView->showPriceBreakdown(price);
    updateCartTabTitle();
}

void CustomerWorkspace::handlePlaceOrder()
{
    OrderPlacementResult result = orderController.placeOnlineOrder(
        buildOrderItems(),
        checkoutView->getRequestedLoyaltyPoints(),
        checkoutView->getRequestedAccountCreditCents());

    if (!result.success) {
        // includes "Only one order can be placed at a time"
        checkoutView->showMessage(result.message, true);
        return;
    }

    checkoutView->clearCart();
    refreshCustomerData();

    QString message = tr("Order #%1 placed. Amount charged: %2. You can follow it on My Dashboard.")
                          .arg(result.order->getOrderNumber())
                          .arg(ViewFormatting::money(result.order->getAmountChargedCents()));
    checkoutView->showMessage(message, false);
}

void CustomerWorkspace::handleEmailUpdate(const QString& email)
{
    ProfileUpdateResult result = customerController.updateEmail(email);
    refreshCustomerData();
    dashboardView->showProfileMessage(result.message, !result.success);
}

void CustomerWorkspace::handleHomeAddressUpdate(const QString& homeAddress)
{
    ProfileUpdateResult result = customerController.updateHomeAddress(homeAddress);
    refreshCustomerData();
    dashboardView->showProfileMessage(result.message, !result.success);
}

void CustomerWorkspace::updateCartTabTitle()
{
    const int count = checkoutView->getCartItemCount();
    const int cartIndex = ui->tabWidget->indexOf(checkoutView);

    if (count > 0) {
        ui->tabWidget->setTabText(cartIndex, tr("Cart / Checkout (%1)").arg(count));
    } else {
        ui->tabWidget->setTabText(cartIndex, tr("Cart / Checkout"));
    }
}

std::vector<OrderItem> CustomerWorkspace::buildOrderItems() const
{
    std::vector<OrderItem> items;

    for (const std::pair<int, int>& line : checkoutView->getCartLines()) {
        std::shared_ptr<MenuItem> menuItem = findMenuItem(line.first);

        // only build items from real catalogue entries
        if (menuItem != nullptr && line.second > 0) {
            items.push_back(OrderItem(menuItem, line.second));
        }
    }

    return items;
}

std::shared_ptr<MenuItem> CustomerWorkspace::findMenuItem(int menuItemId) const
{
    for (const std::shared_ptr<MenuItem>& item : customerController.getMenuItems()) {
        if (item != nullptr && item->getId() == menuItemId) {
            return item;
        }
    }

    return nullptr;
}
