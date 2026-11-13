#ifndef KITCHENQUEUEVIEW_H
#define KITCHENQUEUEVIEW_H

#include <QWidget>

#include <memory>
#include <vector>

class Order;

QT_BEGIN_NAMESPACE
namespace Ui {
  class KitchenQueueView;
}
QT_END_NAMESPACE

// displays the kitchen queue and allows staff to select an order
class KitchenQueueView : public QWidget {
  Q_OBJECT

public:
  explicit KitchenQueueView(QWidget* parent = nullptr);
  ~KitchenQueueView();

  // display pending orders in fifo order
  void showOrders(const std::vector<std::shared_ptr<Order>>& orders);

  // select an order or return false if it is not in the queue
  bool selectOrder(int orderNumber);

  // clear the queue display
  void clear();

signals:
  void orderSelected(int orderNumber);

private slots:
  void on_queueTable_itemSelectionChanged();

private:
  Ui::KitchenQueueView* ui;
};

#endif
