#ifndef STAFFWORKSPACE_H
#define STAFFWORKSPACE_H

#include <QWidget>
#include <QString>

QT_BEGIN_NAMESPACE
namespace Ui { class StaffWorkspace; }
QT_END_NAMESPACE

// Cashier, Manager and System Administrator screen for D1:
// shows only the identified user's name and user type, plus Log Out.
class StaffWorkspace : public QWidget
{
    Q_OBJECT

public:
    explicit StaffWorkspace(QWidget *parent = nullptr);
    ~StaffWorkspace();

    void showUser(const QString& name, const QString& userType);
    void clear();

signals:
    void logoutRequested();

private slots:
    void on_logoutButton_clicked();

private:
    Ui::StaffWorkspace *ui;
};

#endif // STAFFWORKSPACE_H
