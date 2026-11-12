#ifndef LOGINVIEW_H
#define LOGINVIEW_H

#include <QWidget>
#include <QString>

QT_BEGIN_NAMESPACE
namespace Ui { class LoginView; }
QT_END_NAMESPACE

// Startup form: the user types their name or username.
// LoginView holds no controller; it emits loginRequested and MainWindow
// asks ApplicationController to identify the user and pick the workspace.
class LoginView : public QWidget
{
    Q_OBJECT

public:
    explicit LoginView(QWidget *parent = nullptr);
    ~LoginView();

    void showError(const QString& message);
    void clear();

signals:
    void loginRequested(const QString& nameOrUsername);

private slots:
    void on_loginButton_clicked();
    void on_nameLineEdit_returnPressed();

private:
    void submit();

    Ui::LoginView *ui;
};

#endif // LOGINVIEW_H
