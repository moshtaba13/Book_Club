#ifndef FORGOTPASSWORDWIDGET_H
#define FORGOTPASSWORDWIDGET_H
#include <QWidget>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include "NetworkManager.h"

class ForgotPasswordWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ForgotPasswordWidget(QWidget *parent = nullptr);

signals:
    void backToLoginRequested();

private slots:
    void onResetClicked();
    void onNetworkResponse(RequestType type, ResponseStatus status, const QJsonObject &data, const QString &message);

private:
    QLineEdit *leUsername;
    QLineEdit *leAnswer;
    QLineEdit *leNewPassword;
    QLineEdit *leConfirmPassword;
    QPushButton *btnReset;
    QPushButton *btnBack;
};
#endif // FORGOTPASSWORDWIDGET_H