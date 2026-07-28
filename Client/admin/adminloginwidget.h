#ifndef ADMINLOGINWIDGET_H
#define ADMINLOGINWIDGET_H
#include <QWidget>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>

class AdminLoginWidget : public QWidget
{
    Q_OBJECT
public:
    explicit AdminLoginWidget(QWidget *parent = nullptr);

signals:
    void adminSignInSuccess();
    void backToLoginRequested();

protected:
    void paintEvent(QPaintEvent *event) override;

private slots:
    void onLoginClicked();

private:
    QLineEdit *leUsername;
    QLineEdit *lePassword;
    QPushButton *btnLogin;
    QPushButton *btnBack;
};
#endif // ADMINLOGINWIDGET_H