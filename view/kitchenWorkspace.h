#ifndef KITCHENWORKSPACE_H
#define KITCHENWORKSPACE_H

#include <QWidget>
#include <QString>

class KitchenController;
class KitchenQueueView;
class OrderDetailsView;

QT_BEGIN_NAMESPACE
namespace Ui {
  class KitchenWorkspace;
}
QT_END_NAMESPACE

// coordinates the kitchen queue and order details views
class KitchenWorkspace : public QWidget {
  Q_OBJECT

public:
  explicit KitchenWorkspace(KitchenController& kitchenController, QWidget* parent = nullptr);
  ~KitchenWorkspace();

  // load the kitchen screen for the current staff member
  void loadForUser(const QString& staffName);

  // clear the previous staff member's screen
  void clear();

signals:
  void logoutRequested();

private slots:
  void on_logoutButton_clicked();
  void on_refreshButton_clicked();

  // respond to requests from the child views
  void handleOrderSelected(int orderNumber);
  void handleCompleteRequested(int orderNumber);

private:
  // refresh the queue while preserving the selected order when possible
  void refreshQueue();

  Ui::KitchenWorkspace* ui;
  KitchenController& kitchenController;

  KitchenQueueView* queueView;
  OrderDetailsView* detailsView;

  // -1 indicates that no order is selected
  int selectedOrderNumber;
};

#endif
