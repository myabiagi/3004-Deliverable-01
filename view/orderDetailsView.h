#ifndef ORDERDETAILSVIEW_H
#define ORDERDETAILSVIEW_H

#include <QWidget>
#include <QString>

#include <memory>

class Order;

QT_BEGIN_NAMESPACE
namespace Ui {
  class OrderDetailsView;
}
QT_END_NAMESPACE

// displays the selected order and its required ingredients
class OrderDetailsView : public QWidget {
  Q_OBJECT

public:
  explicit OrderDetailsView(QWidget* parent = nullptr);
  ~OrderDetailsView();

  // display an order and whether its completion button should be enabled
  void showOrder(const std::shared_ptr<Order>& order, bool canComplete);

  // display feedback or clear the order details
  void showMessage(const QString& message, bool isError);
  void clear();

signals:
  void completeRequested(int orderNumber);

private slots:
  void on_completeButton_clicked();

private:
  Ui::OrderDetailsView* ui;

  // -1 indicates that no order is currently displayed
  int displayedOrderNumber;
};

#endif
