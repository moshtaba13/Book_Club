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
#include "NetworkManager.h"

class RegisterPublisher : public QWidget {
    Q_OBJECT
public:
    explicit RegisterPublisher(QWidget *parent = nullptr);
    ~RegisterPublisher();
protected:
    void paintEvent(QPaintEvent *event) override;
private slots:
    void onSignUpClicked();
    void onNetworkResponse(RequestType type, ResponseStatus status, const QJsonObject &data, const QString &message);
signals:
    void SignUpSuccess();
    void goToLogin();
private:
    QFrame *signupframe;
    QLabel *lblwelcome;
    QLabel *lblsubtitle;
    QLabel *lblusername;
    QLineEdit *leusername;
    QLabel *lblemail;
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
};
#endif // REGISTERPUBLISHER_H