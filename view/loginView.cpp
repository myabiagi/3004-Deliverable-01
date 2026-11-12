#include "loginView.h"
#include "ui_loginView.h"

LoginView::LoginView(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::LoginView)
{
    ui->setupUi(this);
    ui->errorLabel->clear();
}

LoginView::~LoginView()
{
    delete ui;
}

void LoginView::on_loginButton_clicked()
{
    submit();
}

void LoginView::on_nameLineEdit_returnPressed()
{
    submit();
}

void LoginView::submit()
{
    const QString input = ui->nameLineEdit->text().trimmed();

    if (input.isEmpty()) {
        showError(tr("Please enter your name or username."));
        return;
    }

    ui->errorLabel->clear();
    emit loginRequested(input);
}

void LoginView::showError(const QString& message)
{
    ui->errorLabel->setText(message);
    ui->nameLineEdit->selectAll();
    ui->nameLineEdit->setFocus();
}

void LoginView::clear()
{
    ui->nameLineEdit->clear();
    ui->errorLabel->clear();
    ui->nameLineEdit->setFocus();
}
