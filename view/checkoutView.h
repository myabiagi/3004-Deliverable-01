#ifndef CHECKOUTVIEW_H
#define CHECKOUTVIEW_H

#include <QWidget>
#include <QString>
#include <QtGlobal>

#include <memory>
#include <utility>
#include <vector>

#include "pricingService.h"

class MenuItem;

QT_BEGIN_NAMESPACE
namespace Ui {
  class CheckoutView;
}
QT_END_NAMESPACE

// manages the temporary cart and displays checkout information
class CheckoutView : public QWidget {
  Q_OBJECT

public:
  explicit CheckoutView(QWidget* parent = nullptr);
  ~CheckoutView();

  // cart contents
  void addItem(const std::shared_ptr<MenuItem>& item, int quantity);
  std::vector<std::pair<int, int>> getCartLines() const;
  int getCartItemCount() const;

  // redemption choices
  int getRequestedLoyaltyPoints() const;
  int getRequestedAccountCreditCents() const;
  void setAvailableBalances(int loyaltyPoints, int accountCreditCents);

  void showPriceBreakdown(const PriceBreakdown& price);
  void showMessage(const QString& message, bool isError);

  // empty the cart after placing an order
  void clearCart();

  // reset the checkout on logout
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

  Ui::CheckoutView* ui;
  // each entry stores a menu item id and quantity
  std::vector<std::pair<int, int>> cartLines;
  int availableLoyaltyPoints = 0;
};

#endif