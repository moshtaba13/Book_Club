#include "register.h"
#include <QStyleOption>
#include <QPainter>
#include <QMessageBox>
#include "User.h"

Register::Register(UserManager *userManager, PublisherManager *publisherManager, QWidget *parent)
    : QWidget(parent), userManager(userManager), publisherManager(publisherManager)
{
    // ۱. تنظیم نام آبجکت برای اعمال پس‌زمینه کرمی یکدست روی کل صفحه
    this->setObjectName("signUpPage");
    this->setStyleSheet(
        "QWidget#signUpPage {"
        "   background: qlineargradient("
        "       x1:0, y1:0, x2:1, y2:1,"
        "       stop:0 #FFF8F2,"
        "       stop:0.5 #F2E8DF,"
        "       stop:1 #E8D8C9"
        "   );"
        "}"
        );

    // ۲. لایوت شبکه اصلی برای مرکزچین کردن کارت
    QGridLayout *mainLayout = new QGridLayout(this);
    mainLayout->setContentsMargins(0, 0, 0, 0);

    // ۳. ساخت کارت اصلی (signupframe)
    signupframe = new QFrame(this);
    signupframe->setObjectName("signUpFrame");
    signupframe->setMinimumSize(360, 560);
    signupframe->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    signupframe->setStyleSheet(
        "QFrame#signUpFrame {"
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

    // ۴. لایوت عمودی داخل کارت
    QVBoxLayout *frameLayout = new QVBoxLayout(signupframe);
    frameLayout->setContentsMargins(35, 35, 35, 35);
    frameLayout->setSpacing(12);
    frameLayout->setAlignment(Qt::AlignCenter);

    // ۵. عناوین بالای فرم
    lblwelcome = new QLabel("Welcome to Book Club!", signupframe);
    lblwelcome->setFont(QFont("Segoe UI", 18, QFont::Bold));
    lblwelcome->setAlignment(Qt::AlignCenter);
    frameLayout->addWidget(lblwelcome);

    lblsubtitle = new QLabel("Create your account", signupframe);
    lblsubtitle->setFont(QFont("Segoe UI", 11));
    lblsubtitle->setAlignment(Qt::AlignCenter);
    frameLayout->addWidget(lblsubtitle);

    frameLayout->addSpacing(5);

    // ۶. فیلد Username
    lblusername = new QLabel("Username", signupframe);
    lblusername->setFont(QFont("Segoe UI", 9, QFont::DemiBold));
    frameLayout->addWidget(lblusername);

    leusername = new QLineEdit(signupframe);
    leusername->setPlaceholderText("Choose a username");
    leusername->setMinimumHeight(38);
    frameLayout->addWidget(leusername);

    // ۷. فیلد Email

    // ۸. فیلد Password
    lblpassword = new QLabel("Password", signupframe);
    lblpassword->setFont(QFont("Segoe UI", 9, QFont::DemiBold));
    frameLayout->addWidget(lblpassword);

    lepassword = new QLineEdit(signupframe);
    lepassword->setPlaceholderText("Create a password");
    lepassword->setEchoMode(QLineEdit::Password);
    lepassword->setMinimumHeight(38);
    frameLayout->addWidget(lepassword);

    // ۹. فیلد Confirm Password
    lblconfirmpassword = new QLabel("Confirm Password", signupframe);
    lblconfirmpassword->setFont(QFont("Segoe UI", 9, QFont::DemiBold));
    frameLayout->addWidget(lblconfirmpassword);

    leconfirmpassword = new QLineEdit(signupframe);
    leconfirmpassword->setPlaceholderText("Re-enter your password");
    leconfirmpassword->setEchoMode(QLineEdit::Password);
    leconfirmpassword->setMinimumHeight(38);
    frameLayout->addWidget(leconfirmpassword);

    // ۹-۱. فیلد سؤال امنیتی (برای فراموشی رمز عبور در آینده)
    lblsecurityquestion = new QLabel("What is your favorite book or author?", signupframe);
    lblsecurityquestion->setFont(QFont("Segoe UI", 9, QFont::DemiBold));
    lblsecurityquestion->setWordWrap(true);
    frameLayout->addWidget(lblsecurityquestion);

    lesecurityanswer = new QLineEdit(signupframe);
    lesecurityanswer->setPlaceholderText("Your answer");
    lesecurityanswer->setMinimumHeight(38);
    frameLayout->addWidget(lesecurityanswer);

    // ۱۰. چک‌باکس قوانین
    chkterms = new QCheckBox("I agree to the Terms", signupframe);
    frameLayout->addWidget(chkterms);

    frameLayout->addSpacing(5);

    // ۱۱. دکمه اصلی Sign Up
    btnsignup = new QPushButton("Sign Up", signupframe);
    btnsignup->setObjectName("signUpBtn");
    btnsignup->setMinimumHeight(42);
    btnsignup->setCursor(Qt::PointingHandCursor);
    frameLayout->addWidget(btnsignup);

    frameLayout->addSpacing(5);

    // ۱۲. بخش فوتر برای هدایت به لاگین
    QHBoxLayout *signinLayout = new QHBoxLayout();
    signinLayout->setAlignment(Qt::AlignCenter);
    lblsignin = new QLabel("Already have an account?", signupframe);

    btnsignin = new QPushButton("Sign In", signupframe);
    btnsignin->setStyleSheet("color: #FF69B4; background: transparent; border: none; font-weight: bold; text-decoration: underline;");
    btnsignin->setCursor(Qt::PointingHandCursor);

    signinLayout->addWidget(lblsignin);
    signinLayout->addWidget(btnsignin);
    frameLayout->addLayout(signinLayout);

    // ۱۳. اتصال دکمه سوییچ به سیگنال خروجی کلاس
    connect(btnsignin, &QPushButton::clicked, this, [=]() {
        emit goToLogin();
    });

    // ۱۴. قرار دادن کارت در مرکز صفحه
    mainLayout->addWidget(signupframe, 1, 1);
    mainLayout->setRowStretch(0, 1);
    mainLayout->setRowStretch(2, 1);
    mainLayout->setColumnStretch(0, 1);
    mainLayout->setColumnStretch(2, 1);

    this->setLayout(mainLayout);

    connect(btnsignup, &QPushButton::clicked, this, &Register::onSignUpClicked);
}

void Register::onSignUpClicked() {
    // ۱. بررسی خالی بودن نام کاربری
    if (leusername->text().isEmpty()) {
        QMessageBox::warning(this, "Validation Error", "Please enter a username.");
        return;
    }

    // ۲. بررسی خالی بودن ایمیل


    // ۳. بررسی خالی بودن رمز عبور
    if (lepassword->text().isEmpty()) {
        QMessageBox::warning(this, "Validation Error", "Please enter a password.");
        return;
    }

    // ۴. بررسی خالی بودن تاییدیه رمز عبور
    if (leconfirmpassword->text().isEmpty()) {
        QMessageBox::warning(this, "Validation Error", "Please confirm your password.");
        return;
    }

    // ۵. بررسی تطابق رمز عبور و تاییدیه آن
    if (lepassword->text() != leconfirmpassword->text()) {
        QMessageBox::warning(this, "Validation Error", "Passwords do not match. Please try again.");
        return;
    }

    // ۶. بررسی خالی بودن پاسخ سؤال امنیتی
    if (lesecurityanswer->text().isEmpty()) {
        QMessageBox::warning(this, "Validation Error", "Please answer the security question.");
        return;
    }

    // ۷. بررسی تایید قوانین و شروط
    if (!chkterms->isChecked()) {
        QMessageBox::warning(this, "Validation Error", "You must agree to the Terms and Conditions to register.");
        return;
    }

    // ۸. ساخت کاربر جدید و ثبتش در UserManager
    User newUser(leusername->text().trimmed(), lepassword->text(), lesecurityanswer->text().trimmed());
    newUser.setFirstLogin(true);

    if (userManager->usernameExists(leusername->text().trimmed()) ||
        publisherManager->usernameExists(leusername->text().trimmed())) {
        QMessageBox::warning(this, "Validation Error", "This username is already taken. Please choose another one.");
        return;
    }

    bool success = userManager->registerUser(newUser);
    if (!success) {
        QMessageBox::warning(this, "Validation Error", "Something went wrong. Please try again.");
        return;
    }

    // ۹. نمایش پیام موفقیت و شلیک سیگنال
    QMessageBox::information(this, "Success", "Your account has been created successfully!");

    emit SignUpSuccess();
}

void Register::paintEvent(QPaintEvent *event) {
    QStyleOption opt;
    opt.initFrom(this);
    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}

Register::~Register() {}