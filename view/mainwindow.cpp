#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "applicationController.h"

#include "loginView.h"
#include "customerWorkspace.h"
#include "kitchenWorkspace.h"
#include "staffWorkspace.h"
#include "viewFormatting.h"

MainWindow::MainWindow(ApplicationController& applicationController,
                       CustomerController& customerController,
                       OrderController& orderController,
                       KitchenController& kitchenController,
                       QWidget* parent)
  : QMainWindow(parent),
    ui(new Ui::MainWindow),
    applicationController(applicationController),
    loginView(nullptr),
    customerWorkspace(nullptr),
    kitchenWorkspace(nullptr),
    staffWorkspace(nullptr) {
  ui->setupUi(this);

  // create one instance of each screen
  loginView = new LoginView(this);
  customerWorkspace = new CustomerWorkspace(customerController, orderController, this);
  kitchenWorkspace = new KitchenWorkspace(kitchenController, this);
  staffWorkspace = new StaffWorkspace(this);

  // display only one screen at a time
  ui->stackedWidget->addWidget(loginView);
  ui->stackedWidget->addWidget(customerWorkspace);
  ui->stackedWidget->addWidget(kitchenWorkspace);
  ui->stackedWidget->addWidget(staffWorkspace);

  // connect login and logout requests to the main window
  connect(loginView, &LoginView::loginRequested,
          this, &MainWindow::handleLoginRequested);

  connect(customerWorkspace, &CustomerWorkspace::logoutRequested,
          this, &MainWindow::handleLogoutRequested);

  connect(kitchenWorkspace, &KitchenWorkspace::logoutRequested,
          this, &MainWindow::handleLogoutRequested);

  connect(staffWorkspace, &StaffWorkspace::logoutRequested,
          this, &MainWindow::handleLogoutRequested);

  ui->stackedWidget->setCurrentWidget(loginView);
  statusBar()->showMessage(tr("Not logged in"));
}

MainWindow::~MainWindow() {
  delete ui;
}

void MainWindow::handleLoginRequested(const QString& nameOrUsername) {
  // the application controller identifies the user and selects their workspace
  if (!applicationController.loginAndSelectWorkspace(nameOrUsername)) {
    loginView->showError(tr("No user was found with that name, username, or staff ID."));
    return;
  }

  if (!showWorkspaceForCurrentUser()) {
    applicationController.logoutToLogin();
    loginView->showError(tr("This user type does not have a workspace."));
    return;
  }

  loginView->clear();
}

bool MainWindow::showWorkspaceForCurrentUser() {
  std::shared_ptr<User> user = applicationController.getCurrentUser();
  UserRole role;

  if (user == nullptr || !applicationController.getCurrentUserRole(role)) {
    return false;
  }

  const QString roleText = ViewFormatting::roleName(role);

  // display the workspace associated with the current user's role
  switch (applicationController.getCurrentWorkspace()) {
    case WorkspaceType::Customer:
      customerWorkspace->loadForCurrentUser();
      ui->stackedWidget->setCurrentWidget(customerWorkspace);
      break;

    case WorkspaceType::Kitchen:
      kitchenWorkspace->loadForUser(user->getName());
      ui->stackedWidget->setCurrentWidget(kitchenWorkspace);
      break;

    case WorkspaceType::Staff:
      staffWorkspace->showUser(user->getName(), roleText);
      ui->stackedWidget->setCurrentWidget(staffWorkspace);
      break;

    case WorkspaceType::Login:
      return false;
  }

  statusBar()->showMessage(tr("Logged in as %1 (%2)").arg(user->getName(), roleText));
  return true;
}

void MainWindow::handleLogoutRequested() {
  // end the session without deleting orders or customer balances
  applicationController.logoutToLogin();

  clearAllScreens();
  ui->stackedWidget->setCurrentWidget(loginView);
  statusBar()->showMessage(tr("Not logged in"));
}

void MainWindow::clearAllScreens() {
  loginView->clear();
  customerWorkspace->clear();
  kitchenWorkspace->clear();
  staffWorkspace->clear();
}
