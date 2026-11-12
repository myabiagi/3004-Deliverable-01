#include "staffWorkspace.h"
#include "ui_staffWorkspace.h"

StaffWorkspace::StaffWorkspace(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::StaffWorkspace)
{
    ui->setupUi(this);
    clear();
}

StaffWorkspace::~StaffWorkspace()
{
    delete ui;
}

void StaffWorkspace::showUser(const QString& name, const QString& userType)
{
    ui->nameValueLabel->setText(name);
    ui->userTypeValueLabel->setText(userType);
}

void StaffWorkspace::clear()
{
    ui->nameValueLabel->clear();
    ui->userTypeValueLabel->clear();
}

void StaffWorkspace::on_logoutButton_clicked()
{
    emit logoutRequested();
}
