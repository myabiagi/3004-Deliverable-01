#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QString>

class ApplicationController;
class CustomerController;
class OrderController;
class KitchenController;

class LoginView;
class CustomerWorkspace;
class KitchenWorkspace;
class StaffWorkspace;

QT_BEGIN_NAMESPACE
namespace Ui {
  class MainWindow;
}
QT_END_NAMESPACE

// displays the login screen or the workspace for the current user
class MainWindow : public QMainWindow {
  Q_OBJECT

public:
  // use the existing controllers to coordinate navigation and workspaces
  MainWindow(ApplicationController& applicationController,
             CustomerController& customerController,
             OrderController& orderController,
             KitchenController& kitchenController,
             QWidget* parent = nullptr);
  ~MainWindow();

private slots:
  // respond to login and logout requests from the views
  void handleLoginRequested(const QString& nameOrUsername);
  void handleLogoutRequested();

private:
  // display the workspace selected by the application controller
  bool showWorkspaceForCurrentUser();

  // clear information belonging to the previous user
  void clearAllScreens();

  Ui::MainWindow* ui;
  ApplicationController& applicationController;

  LoginView* loginView;
  CustomerWorkspace* customerWorkspace;
  KitchenWorkspace* kitchenWorkspace;
  StaffWorkspace* staffWorkspace;
};

#endif
