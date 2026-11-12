#ifndef CHECKOUTVIEW_H
#define CHECKOUTVIEW_H

#include <QWidget>
#include <QString>
#include <QtGlobal>

#include <memory>
#include <utility>
#include <vector>

#include "pricingService.h" // PriceBreakdown (display only)

class MenuItem;

QT_BEGIN_NAMESPACE
namespace Ui { class CheckoutView; }
QT_END_NAMESPACE

// Cart and checkout. Keeps the temporary cart as (menu item id, quantity) pairs,
// collects the loyalty points / account credit the customer wants to use, and shows
// the totals CustomerWorkspace gets back from OrderController.
class CheckoutView : public QWidget
{
    Q_OBJECT

public:
    explicit CheckoutView(QWidget *parent = nullptr);
    ~CheckoutView();

    // cart contents
    void addItem(const std::shared_ptr<MenuItem>& item, int quantity);
    std::vector<std::pair<int, int>> getCartLines() const; // (menu item id, quantity)
    int getCartItemCount() const;

    // redemption choices
    int getRequestedLoyaltyPoints() const;
    qint64 getRequestedAccountCreditCents() const;
    void setAvailableBalances(int loyaltyPoints, qint64 accountCreditCents);

    void showPriceBreakdown(const PriceBreakdown& price);
    void showMessage(const QString& message, bool isError);

    // empty the cart after a successful order (keeps the message)
    void clearCart();

    // reset everything on logout
    void clear();

signals:
    void checkoutChanged();
    void placeOrderRequested();

private slots:
    void on_removeItemButton_clicked();
    void on_clearCartButton_clicked();
    void on_pointsSpinBox_valueChanged(int value);
    void on_creditSpinBox_valueChanged(double value);
    void on_placeOrderButton_clicked();

private:
    void updateRow(int row);

    Ui::CheckoutView *ui;
    std::vector<std::pair<int, int>> cartLines; // same order as the table rows
};

#endif // CHECKOUTVIEW_H
