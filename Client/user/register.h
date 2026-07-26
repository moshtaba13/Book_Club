#ifndef REGISTER_H
#define REGISTER_H
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
#include "UserManager.h"
#include "PublisherManager.h"

class Register : public QWidget {
    Q_OBJECT
public:
    explicit Register(UserManager *userManager, PublisherManager *publisherManager, QWidget *parent = nullptr);
    ~Register();
protected:
    void paintEvent(QPaintEvent *event) override;
private slots:
    void onSignUpClicked();
signals:
    void SignUpSuccess();
    void goToLogin();
private:
    UserManager *userManager;

    QFrame *signupframe;
    QLabel *lblwelcome;
    QLabel *lblsubtitle;
    QLabel *lblusername;
    QLineEdit *leusername;
    QLineEdit *leemail;
    QLabel *lblpassword;
    QLineEdit *lepassword;
    QLabel *lblconfirmpassword;
    QLineEdit *leconfirmpassword;
    QLabel *lblsecurityquestion;
    QLineEdit *lesecurityanswer;
    QCheckBox *chkterms;
    QPushButton *btnsignup;
    QLabel *lblsignin;
    QPushButton *btnsignin;
    PublisherManager *publisherManager;
};
#endif // REGISTER_H