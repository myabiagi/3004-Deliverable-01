#ifndef STAFFWORKSPACE_H
#define STAFFWORKSPACE_H

#include <QWidget>
#include <QString>

QT_BEGIN_NAMESPACE
namespace Ui {
  class StaffWorkspace;
}
QT_END_NAMESPACE

// displays basic information for cashiers, managers, and administrators
class StaffWorkspace : public QWidget {
  Q_OBJECT

public:
  explicit StaffWorkspace(QWidget* parent = nullptr);
  ~StaffWorkspace();

  // display the current staff member's name and role
  void showUser(const QString& name, const QString& userType);

  // clear staff information on logout
  void clear();

signals:
  void logoutRequested();

private slots:
  void on_logoutButton_clicked();

private:
  Ui::StaffWorkspace* ui;
};

#endif
