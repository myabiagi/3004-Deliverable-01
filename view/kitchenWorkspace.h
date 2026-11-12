#ifndef KITCHENWORKSPACE_H
#define KITCHENWORKSPACE_H

#include <QWidget>
#include <QString>

class KitchenController;
class KitchenQueueView;
class OrderDetailsView;

QT_BEGIN_NAMESPACE
namespace Ui { class KitchenWorkspace; }
QT_END_NAMESPACE

// Kitchen staff screen. Owns the order queue and order details views and is the
// only kitchen view that talks to KitchenController.
class KitchenWorkspace : public QWidget
{
    Q_OBJECT

public:
    explicit KitchenWorkspace(KitchenController& kitchenController, QWidget *parent = nullptr);
    ~KitchenWorkspace();

    void loadForUser(const QString& staffName);
    void clear();

signals:
    void logoutRequested();

private slots:
    void on_logoutButton_clicked();
    void on_refreshButton_clicked();

    void handleOrderSelected(int orderNumber);
    void handleCompleteRequested(int orderNumber);

private:
    // reload the FIFO queue and keep the selected order (or the front order) in the details pane
    void refreshQueue();

    Ui::KitchenWorkspace *ui;
    KitchenController& kitchenController;

    KitchenQueueView* queueView;
    OrderDetailsView* detailsView;

    int selectedOrderNumber; // -1 when nothing is selected
};

#endif // KITCHENWORKSPACE_H
