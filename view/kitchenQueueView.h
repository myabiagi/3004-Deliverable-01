#ifndef KITCHENQUEUEVIEW_H
#define KITCHENQUEUEVIEW_H

#include <QWidget>

#include <memory>
#include <vector>

class Order;

QT_BEGIN_NAMESPACE
namespace Ui { class KitchenQueueView; }
QT_END_NAMESPACE

// FIFO kitchen queue: order number, customer name and order date/time.
// Emits orderSelected when the kitchen staff clicks a row.
class KitchenQueueView : public QWidget
{
    Q_OBJECT

public:
    explicit KitchenQueueView(QWidget *parent = nullptr);
    ~KitchenQueueView();

    void showOrders(const std::vector<std::shared_ptr<Order>>& orders);

    // select the row for this order; returns false if it is not in the queue
    bool selectOrder(int orderNumber);

    void clear();

signals:
    void orderSelected(int orderNumber);

private slots:
    void on_queueTable_itemSelectionChanged();

private:
    Ui::KitchenQueueView *ui;
};

#endif // KITCHENQUEUEVIEW_H
