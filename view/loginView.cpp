#include "loginView.h"
#include "ui_loginView.h"

LoginView::LoginView(QWidget* parent)
  : QWidget(parent), ui(new Ui::LoginView) {
  ui->setupUi(this);
  ui->errorLabel->clear();
}

LoginView::~LoginView() {
  delete ui;
}

void LoginView::on_loginButton_clicked() {
  submit();
}

void LoginView::on_nameLineEdit_returnPressed() {
  submit();
}

void LoginView::submit() {
  const QString input = ui->nameLineEdit->text().trimmed();

  if (input.isEmpty()) {
    showError(tr("Please enter your full name, username, or staff ID."));
    return;
  }

  ui->errorLabel->clear();
  emit loginRequested(input);
}

void LoginView::showError(const QString& message) {
  ui->nameLineEdit->clear();
  ui->errorLabel->setText(message);
  ui->nameLineEdit->setFocus();
}

void LoginView::clear() {
  ui->nameLineEdit->clear();
  ui->errorLabel->clear();
  ui->nameLineEdit->setFocus();
}

void LoginView::on_nameLineEdit_textChanged(const QString&) {
  // hide the previous error when the user starts typing again
  ui->errorLabel->clear();
}