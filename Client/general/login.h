#ifndef LOGIN_H
#define LOGIN_H
#include <QWidget>
#include <QFrame>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QCheckBox>
#include <QVBoxLayout>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QPaintEvent>
#include "home.h"
#include "NetworkManager.h"

class login : public QWidget
{
    Q_OBJECT
public:
    explicit login(QWidget *parent = nullptr);
    ~login();
protected:
    void paintEvent(QPaintEvent *event) override;
private slots:
    void onSignInClicked();
    void onNetworkResponse(RequestType type, ResponseStatus status, const QJsonObject &data, const QString &message);
signals:
    void UserLoginSuccess(const QJsonObject &userData);
    void PublisherLoginSuccess(const QJsonObject &publisherData);
    void GoToSignUp();
    void GoToSignUpPublisher();
    void ForgotPasswordRequested();
    void EnterAsAdminRequested();
private:
    QFrame *loginframe;
    QLabel *lblwelcome;
    QLabel *lblsubtitle;
    QLabel *lblusername;
    QLineEdit *leusername;
    QLabel *lblpassword;
    QLineEdit *lepassword;
    QCheckBox *chkrememberme;
    QPushButton *btnforgot;
    QPushButton *btnsignin;
    QLabel *lblsignup;
    QPushButton *btnsignup;
    QPushButton *btnsignupPublisher;
    QPushButton *btnEnterAsAdmin;
};
#endif // LOGIN_H