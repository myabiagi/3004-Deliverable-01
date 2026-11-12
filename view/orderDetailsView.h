#ifndef ORDERDETAILSVIEW_H
#define ORDERDETAILSVIEW_H

#include <QWidget>
#include <QString>

#include <memory>

class Order;

QT_BEGIN_NAMESPACE
namespace Ui { class OrderDetailsView; }
QT_END_NAMESPACE

// Details of one queued order for the kitchen: items purchased and the
// ingredients each one needs, plus the "Mark as Completed" action.
class OrderDetailsView : public QWidget
{
    Q_OBJECT

public:
    explicit OrderDetailsView(QWidget *parent = nullptr);
    ~OrderDetailsView();

    // canComplete is true only for the order at the front of the queue
    void showOrder(const std::shared_ptr<Order>& order, bool canComplete);
    void showMessage(const QString& message, bool isError);
    void clear();

signals:
    void completeRequested(int orderNumber);

private slots:
    void on_completeButton_clicked();

private:
    Ui::OrderDetailsView *ui;
    int displayedOrderNumber; // -1 when no order is shown
};

#endif // ORDERDETAILSVIEW_H
