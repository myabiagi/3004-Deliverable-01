#ifndef LOGINVIEW_H
#define LOGINVIEW_H

#include <QWidget>
#include <QString>

QT_BEGIN_NAMESPACE
namespace Ui {
  class LoginView;
}
QT_END_NAMESPACE

// collects a full name, username, or staff ID for identification
class LoginView : public QWidget {
  Q_OBJECT

public:
  explicit LoginView(QWidget* parent = nullptr);
  ~LoginView();

  void showError(const QString& message);
  void clear();

signals:
  void loginRequested(const QString& nameOrUsername);

private slots:
  void on_loginButton_clicked();
  void on_nameLineEdit_returnPressed();
  void on_nameLineEdit_textChanged(const QString& text);

private:
  void submit();

  Ui::LoginView* ui;
};

#endif