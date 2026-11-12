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
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

// The single application window. It shows one screen at a time in a QStackedWidget:
// LoginView first, then only the workspace that matches the identified user's type.
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    // the feature controllers are only passed through to the workspaces that own them;
    // MainWindow itself keeps only the ApplicationController for navigation
    MainWindow(ApplicationController& applicationController,
               CustomerController& customerController,
               OrderController& orderController,
               KitchenController& kitchenController,
               QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void handleLoginRequested(const QString& nameOrUsername);
    void handleLogoutRequested();

private:
    // switch the stacked widget to the workspace chosen by ApplicationController
    bool showWorkspaceForCurrentUser();

    // wipe everything the previous user could see
    void clearAllScreens();

    Ui::MainWindow *ui;
    ApplicationController& applicationController;

    LoginView* loginView;
    CustomerWorkspace* customerWorkspace;
    KitchenWorkspace* kitchenWorkspace;
    StaffWorkspace* staffWorkspace;
};
#endif // MAINWINDOW_H
