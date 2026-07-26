#include "registerpublisher.h"
#include <QStyleOption>
#include <QPainter>
#include <QMessageBox>
#include "Publisher.h"

RegisterPublisher::RegisterPublisher(PublisherManager *publisherManager, UserManager *userManager, QWidget *parent)
    : QWidget(parent), publisherManager(publisherManager), userManager(userManager)
{
    this->setObjectName("signUpPublisherPage");
    this->setStyleSheet(
        "QWidget#signUpPublisherPage {"
        "   background: qlineargradient("
        "       x1:0, y1:0, x2:1, y2:1,"
        "       stop:0 #FFF8F2,"
        "       stop:0.5 #F2E8DF,"
        "       stop:1 #E8D8C9"
        "   );"
        "}"
        );

    QGridLayout *mainLayout = new QGridLayout(this);
    mainLayout->setContentsMargins(0, 0, 0, 0);

    signupframe = new QFrame(this);
    signupframe->setObjectName("signUpPublisherFrame");
    signupframe->setMinimumSize(360, 600);
    signupframe->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    signupframe->setStyleSheet(
        "QFrame#signUpPublisherFrame {"
        "   background-color: rgba(210, 235, 255, 170);"
        "   border-radius: 25px;"
        "}"
        "QLabel {"
        "   color: #2C3E50;"
        "   background: transparent;"
        "   border: none;"
        "}"
        "QCheckBox {"
        "   color: #2C3E50;"
        "   background: transparent;"
        "   border: none;"
        "}"
        "QCheckBox::indicator {"
        "   width: 16px;"
        "   height: 16px;"
        "   background-color: #FFFFFF;"
        "   border: 2px solid #000000;"
        "   border-radius: 4px;"
        "}"
        "QCheckBox::indicator:checked {"
        "   background-color: #000000;"
        "}"
        "QLineEdit {"
        "   background-color: rgba(255, 255, 255, 210);"
        "   border: 1.5px solid #FFC0CB;"
        "   border-radius: 8px;"
        "   padding: 8px;"
        "   color: #2C3E50;"
        "}"
        "QLineEdit:focus {"
        "   border: 2px solid #FFB6C1;"
        "}"
        "QPushButton#signUpBtn {"
        "   background-color: rgba(255,255,255,220);"
        "   color: #2C3E50;"
        "   border: 1.5px solid #FFC0CB;"
        "   border-radius: 8px;"
        "   font-weight: bold;"
        "}"
        "QPushButton#signUpBtn:hover { background-color: #FFC0CB; color: white; }"
        "QPushButton#signUpBtn:pressed { background-color: #FFB6C1; }"
        );

    QVBoxLayout *frameLayout = new QVBoxLayout(signupframe);
    frameLayout->setContentsMargins(35, 35, 35, 35);
    frameLayout->setSpacing(12);
    frameLayout->setAlignment(Qt::AlignCenter);

    lblwelcome = new QLabel("Become a Publisher!", signupframe);
    lblwelcome->setFont(QFont("Segoe UI", 18, QFont::Bold));
    lblwelcome->setAlignment(Qt::AlignCenter);
    frameLayout->addWidget(lblwelcome);

    lblsubtitle = new QLabel("Create your publisher account", signupframe);
    lblsubtitle->setFont(QFont("Segoe UI", 11));
    lblsubtitle->setAlignment(Qt::AlignCenter);
    frameLayout->addWidget(lblsubtitle);

    frameLayout->addSpacing(5);

    lblusername = new QLabel("Username", signupframe);
    lblusername->setFont(QFont("Segoe UI", 9, QFont::DemiBold));
    frameLayout->addWidget(lblusername);

    leusername = new QLineEdit(signupframe);
    leusername->setPlaceholderText("Choose a username");
    leusername->setMinimumHeight(38);
    frameLayout->addWidget(leusername);



    lblpassword = new QLabel("Password", signupframe);
    lblpassword->setFont(QFont("Segoe UI", 9, QFont::DemiBold));
    frameLayout->addWidget(lblpassword);

    lepassword = new QLineEdit(signupframe);
    lepassword->setPlaceholderText("Create a password");
    lepassword->setEchoMode(QLineEdit::Password);
    lepassword->setMinimumHeight(38);
    frameLayout->addWidget(lepassword);

    lblconfirmpassword = new QLabel("Confirm Password", signupframe);
    lblconfirmpassword->setFont(QFont("Segoe UI", 9, QFont::DemiBold));
    frameLayout->addWidget(lblconfirmpassword);

    leconfirmpassword = new QLineEdit(signupframe);
    leconfirmpassword->setPlaceholderText("Re-enter your password");
    leconfirmpassword->setEchoMode(QLineEdit::Password);
    leconfirmpassword->setMinimumHeight(38);
    frameLayout->addWidget(leconfirmpassword);

    lblsecurityquestion = new QLabel("What is your favorite book or author?", signupframe);
    lblsecurityquestion->setFont(QFont("Segoe UI", 9, QFont::DemiBold));
    lblsecurityquestion->setWordWrap(true);
    frameLayout->addWidget(lblsecurityquestion);

    lesecurityanswer = new QLineEdit(signupframe);
    lesecurityanswer->setPlaceholderText("Your answer");
    lesecurityanswer->setMinimumHeight(38);
    frameLayout->addWidget(lesecurityanswer);

    chkterms = new QCheckBox("I agree to the Terms", signupframe);
    frameLayout->addWidget(chkterms);

    frameLayout->addSpacing(5);

    btnsignup = new QPushButton("Sign Up", signupframe);
    btnsignup->setObjectName("signUpBtn");
    btnsignup->setMinimumHeight(42);
    btnsignup->setCursor(Qt::PointingHandCursor);
    frameLayout->addWidget(btnsignup);

    frameLayout->addSpacing(5);

    QHBoxLayout *signinLayout = new QHBoxLayout();
    signinLayout->setAlignment(Qt::AlignCenter);
    lblsignin = new QLabel("Already have an account?", signupframe);

    btnsignin = new QPushButton("Sign In", signupframe);
    btnsignin->setStyleSheet("color: #FF69B4; background: transparent; border: none; font-weight: bold; text-decoration: underline;");
    btnsignin->setCursor(Qt::PointingHandCursor);

    signinLayout->addWidget(lblsignin);
    signinLayout->addWidget(btnsignin);
    frameLayout->addLayout(signinLayout);

    connect(btnsignin, &QPushButton::clicked, this, [=]() {
        emit goToLogin();
    });

    mainLayout->addWidget(signupframe, 1, 1);
    mainLayout->setRowStretch(0, 1);
    mainLayout->setRowStretch(2, 1);
    mainLayout->setColumnStretch(0, 1);
    mainLayout->setColumnStretch(2, 1);

    this->setLayout(mainLayout);

    connect(btnsignup, &QPushButton::clicked, this, &RegisterPublisher::onSignUpClicked);
}

void RegisterPublisher::onSignUpClicked() {
    if (leusername->text().isEmpty()) {
        QMessageBox::warning(this, "Validation Error", "Please enter a username.");
        return;
    }

    if (lepassword->text().isEmpty()) {
        QMessageBox::warning(this, "Validation Error", "Please enter a password.");
        return;
    }
    if (leconfirmpassword->text().isEmpty()) {
        QMessageBox::warning(this, "Validation Error", "Please confirm your password.");
        return;
    }
    if (lepassword->text() != leconfirmpassword->text()) {
        QMessageBox::warning(this, "Validation Error", "Passwords do not match. Please try again.");
        return;
    }
    if (lesecurityanswer->text().isEmpty()) {
        QMessageBox::warning(this, "Validation Error", "Please answer the security question.");
        return;
    }
    if (!chkterms->isChecked()) {
        QMessageBox::warning(this, "Validation Error", "You must agree to the Terms and Conditions to register.");
        return;
    }

    QString username = leusername->text().trimmed();

    if (userManager->usernameExists(username) || publisherManager->usernameExists(username)) {
        QMessageBox::warning(this, "Validation Error", "This username is already taken. Please choose another one.");
        return;
    }

    Publisher newPublisher(username, lepassword->text(), lesecurityanswer->text().trimmed());


    bool success = publisherManager->registerPublisher(newPublisher);

    if (!success) {
        QMessageBox::warning(this, "Validation Error", "Something went wrong. Please try again.");
        return;
    }

    QMessageBox::information(this, "Success", "Your publisher account has been created successfully!");
    emit SignUpSuccess();
}

void RegisterPublisher::paintEvent(QPaintEvent *event) {
    QStyleOption opt;
    opt.initFrom(this);
    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}

RegisterPublisher::~RegisterPublisher() {}