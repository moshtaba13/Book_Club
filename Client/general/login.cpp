#include "login.h"
#include <QStyleOption>
#include <QPainter>
#include <QMessageBox>
#include "home.h"

login::login(UserManager *userManager, PublisherManager *publisherManager, QWidget *parent)
    : QWidget(parent), userManager(userManager), publisherManager(publisherManager)
{
    this->setObjectName("loginPage");
    this->setStyleSheet(
        "QWidget#loginPage {"
        "   border-image: url(':/resources/images/login_bg.jpg') 0 0 0 0 stretch stretch;"
        "   background-color: #121212;"
        "}"
        );

    QGridLayout *mainLayout = new QGridLayout(this);
    mainLayout->setContentsMargins(0, 0, 0, 0);

    loginframe = new QFrame(this);
    loginframe->setObjectName("loginFrame");

    loginframe->setMinimumSize(350, 480);
    loginframe->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    loginframe->setStyleSheet(
        "QFrame#loginFrame {"
        "   background-color: rgba(255, 255, 255, 150);"
        "   border-radius: 25px;"
        "   border: 1px solid rgba(255, 255, 255, 100);"
        "}"
        "QLabel, QCheckBox {"
        "   color: #000000;"
        "   background: transparent;"
        "   border: none;"
        "}"
        "QLineEdit {"
        "   background-color: rgba(255, 255, 255, 220);"
        "   border: 1px solid #CCCCCC;"
        "   border-radius: 8px;"
        "   padding: 8px;"
        "   color: #000000;"
        "}"
        "QLineEdit:focus {"
        "   border: 1.5px solid #6F4E37;"
        "}"
        "QPushButton#signInBtn {"
        "   background-color: #6F4E37;"
        "   color: white;"
        "   border-radius: 8px;"
        "   font-weight: bold;"
        "}"
        "QPushButton#signInBtn:hover { background-color: #A33A4A; }"
        );

    QVBoxLayout *frameLayout = new QVBoxLayout(loginframe);
    frameLayout->setContentsMargins(35, 40, 35, 40);
    frameLayout->setSpacing(15);
    frameLayout->setAlignment(Qt::AlignCenter);

    lblwelcome = new QLabel("welcome to Book Club!", loginframe);
    QFont welcomeFont("Segoe UI", 20, QFont::Bold);
    lblwelcome->setFont(welcomeFont);
    lblwelcome->setAlignment(Qt::AlignCenter);
    frameLayout->addWidget(lblwelcome);

    lblsubtitle = new QLabel("sign in to continue", loginframe);
    lblsubtitle->setFont(QFont("Segoe UI", 11));
    lblsubtitle->setAlignment(Qt::AlignCenter);
    frameLayout->addWidget(lblsubtitle);

    frameLayout->addSpacing(10);

    lblusername = new QLabel("username", loginframe);
    lblusername->setFont(QFont("Segoe UI", 10, QFont::DemiBold));
    frameLayout->addWidget(lblusername);

    leusername = new QLineEdit(loginframe);
    leusername->setPlaceholderText("Enter your username");
    leusername->setMinimumHeight(40);
    frameLayout->addWidget(leusername);

    lblpassword = new QLabel("password", loginframe);
    lblpassword->setFont(QFont("Segoe UI", 10, QFont::DemiBold));
    frameLayout->addWidget(lblpassword);

    lepassword = new QLineEdit(loginframe);
    lepassword->setPlaceholderText("Enter your password");
    lepassword->setEchoMode(QLineEdit::Password);
    lepassword->setMinimumHeight(40);
    frameLayout->addWidget(lepassword);

    QHBoxLayout *optionsLayout = new QHBoxLayout();
    chkrememberme = new QCheckBox("remember me", loginframe);

    btnforgot = new QPushButton("forgot password?", loginframe);
    btnforgot->setStyleSheet("color: #6F4E37; background: transparent; border: none; text-decoration: underline;");
    btnforgot->setCursor(Qt::PointingHandCursor);

    optionsLayout->addWidget(chkrememberme);
    optionsLayout->addStretch();
    optionsLayout->addWidget(btnforgot);
    frameLayout->addLayout(optionsLayout);

    frameLayout->addSpacing(15);

    btnsignin = new QPushButton("Sign in", loginframe);
    btnsignin->setObjectName("signInBtn");
    btnsignin->setMinimumHeight(45);
    btnsignin->setCursor(Qt::PointingHandCursor);
    QFont btnFont = btnsignin->font();
    btnFont.setPointSize(12);
    btnsignin->setFont(btnFont);
    frameLayout->addWidget(btnsignin);

    connect(btnsignin, &QPushButton::clicked,
            this, &login::onSignInClicked);

    frameLayout->addSpacing(10);

    QHBoxLayout *signupLayout = new QHBoxLayout();
    signupLayout->setAlignment(Qt::AlignCenter);
    lblsignup = new QLabel("Don't have an account?", loginframe);
    btnsignup = new QPushButton("sign up", loginframe);
    btnsignup->setStyleSheet("color: #6F4E37; background: transparent; border: none; font-weight: bold;");
    btnsignup->setCursor(Qt::PointingHandCursor);

    signupLayout->addWidget(lblsignup);
    signupLayout->addWidget(btnsignup);
    frameLayout->addLayout(signupLayout);
    connect(btnsignup, &QPushButton::clicked, this, [=]() {
        emit GoToSignUp();
    });

    btnsignupPublisher = new QPushButton("Sign up as Publisher", loginframe);
    btnsignupPublisher->setStyleSheet("color: #6F4E37; background: transparent; border: none; font-weight: bold; text-decoration: underline;");
    btnsignupPublisher->setCursor(Qt::PointingHandCursor);
    frameLayout->addWidget(btnsignupPublisher, 0, Qt::AlignCenter);
    connect(btnsignupPublisher, &QPushButton::clicked, this, [=]() {
        emit GoToSignUpPublisher();
    });

    btnEnterAsAdmin = new QPushButton("Enter as Admin", loginframe);
    btnEnterAsAdmin->setStyleSheet("color: #A33A4A; background: transparent; border: none; font-size: 11px;");
    btnEnterAsAdmin->setCursor(Qt::PointingHandCursor);
    frameLayout->addWidget(btnEnterAsAdmin, 0, Qt::AlignCenter);
    connect(btnEnterAsAdmin, &QPushButton::clicked, this, [=]() {
        emit EnterAsAdminRequested();
    });

    mainLayout->addWidget(loginframe, 1, 1);
    mainLayout->setRowStretch(0, 1);
    mainLayout->setRowStretch(2, 1);
    mainLayout->setColumnStretch(0, 1);
    mainLayout->setColumnStretch(2, 1);

    this->setLayout(mainLayout);


    connect(btnforgot, &QPushButton::clicked, this, &login::ForgotPasswordRequested);
}

void login::paintEvent(QPaintEvent *event) {
    QStyleOption opt;
    opt.initFrom(this);
    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}

void login::onSignInClicked()
{
    QString username = leusername->text().trimmed();
    QString password = lepassword->text();

    if (username.isEmpty())
    {
        QMessageBox::warning(this, "Login", "Please enter your username.");
        leusername->setFocus();
        return;
    }

    if (password.isEmpty())
    {
        QMessageBox::warning(this, "Login", "Please enter your password.");
        lepassword->setFocus();
        return;
    }

    User foundUser;
    if (userManager->authenticate(username, password, foundUser)) {
        if (foundUser.isBlocked()) {
            QMessageBox::warning(this, "Login", "Your account has been blocked. Please contact support.");
            return;
        }
        emit SignInSuccess(foundUser);
        return;
    }

    Publisher foundPublisher;
    if (publisherManager->authenticate(username, password, foundPublisher)) {
        if (foundPublisher.isBlocked()) {
            QMessageBox::warning(this, "Login", "Your account has been blocked. Please contact support.");
            return;
        }
        emit PublisherSignInSuccess(foundPublisher);
        return;
    }

    QMessageBox::warning(this, "Login", "Incorrect username or password.");
}

login::~login() {
}