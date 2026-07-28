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
#include <QAction>
#include "home.h"
#include "User.h"
#include "Publisher.h"

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
signals:
    void SignInSuccess(User user);
    void PublisherSignInSuccess(Publisher publisher);
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