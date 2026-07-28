#ifndef FORGOTPASSWORDWIDGET_H
#define FORGOTPASSWORDWIDGET_H
#include <QWidget>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include "UserManager.h"
#include <QPainter>
#include <QStyleOption>

class ForgotPasswordWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ForgotPasswordWidget(UserManager *manager, QWidget *parent = nullptr);
protected:
    void paintEvent(QPaintEvent *event) override;

signals:
    void backToLoginRequested();

private slots:
    void onFindUserClicked();
    void onResetClicked();

private:
    UserManager *userManager;
    QString currentUsername;

    QLineEdit *leUsername;
    QPushButton *btnFindUser;

    QLabel *lblQuestion;
    QLineEdit *leAnswer;
    QLineEdit *leNewPassword;
    QLineEdit *leConfirmPassword;
    QPushButton *btnReset;
    QPushButton *btnBack;
};
#endif // FORGOTPASSWORDWIDGET_H