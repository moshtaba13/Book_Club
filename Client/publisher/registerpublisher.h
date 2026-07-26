#ifndef REGISTERPUBLISHER_H
#define REGISTERPUBLISHER_H
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
#include "PublisherManager.h"
#include "UserManager.h"

class RegisterPublisher : public QWidget {
    Q_OBJECT
public:
    explicit RegisterPublisher(PublisherManager *publisherManager, UserManager *userManager, QWidget *parent = nullptr);
    ~RegisterPublisher();
protected:
    void paintEvent(QPaintEvent *event) override;
private slots:
    void onSignUpClicked();
signals:
    void SignUpSuccess();
    void goToLogin();
private:
    PublisherManager *publisherManager;
    UserManager *userManager;

    QFrame *signupframe;
    QLabel *lblwelcome;
    QLabel *lblsubtitle;
    QLabel *lblusername;
    QLineEdit *leusername;

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
};
#endif // REGISTERPUBLISHER_H