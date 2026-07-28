#include "adminloginwidget.h"
#include <QVBoxLayout>
#include <QMessageBox>
#include <QStyleOption>
#include <QPainter>
#include "networkclient.h"

AdminLoginWidget::AdminLoginWidget(QWidget *parent) : QWidget(parent)
{
    this->setObjectName("adminLoginPage");
    this->setStyleSheet("QWidget#adminLoginPage { background-color: #FFF8F2; }");

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->setContentsMargins(80, 100, 80, 100);
    layout->setSpacing(15);
    layout->setAlignment(Qt::AlignTop);

    QLabel *title = new QLabel("Admin Access", this);
    title->setFont(QFont("Segoe UI", 20, QFont::Bold));
    title->setStyleSheet("color: #2C3E50; background: transparent;");
    layout->addWidget(title);

    leUsername = new QLineEdit(this);
    leUsername->setPlaceholderText("Admin username");
    leUsername->setMinimumHeight(38);
    leUsername->setStyleSheet("background-color: rgba(255,255,255,220); border: 1px solid #FFC0CB; border-radius: 8px; padding: 6px; color: #2C3E50;");
    layout->addWidget(leUsername);

    lePassword = new QLineEdit(this);
    lePassword->setPlaceholderText("Password");
    lePassword->setEchoMode(QLineEdit::Password);
    lePassword->setMinimumHeight(38);
    lePassword->setStyleSheet("background-color: rgba(255,255,255,220); border: 1px solid #FFC0CB; border-radius: 8px; padding: 6px; color: #2C3E50;");
    layout->addWidget(lePassword);

    btnLogin = new QPushButton("Enter", this);
    btnLogin->setCursor(Qt::PointingHandCursor);
    btnLogin->setMinimumHeight(42);
    btnLogin->setStyleSheet(
        "QPushButton { background-color: #2C3E50; color: white; border-radius: 8px; font-weight: bold; }"
        "QPushButton:hover { background-color: #FFC0CB; color: #2C3E50; }"
        );
    layout->addWidget(btnLogin);

    btnBack = new QPushButton("Back to Login", this);
    btnBack->setStyleSheet("color: #6F4E37; background: transparent; border: none; font-weight: bold;");
    btnBack->setCursor(Qt::PointingHandCursor);
    layout->addWidget(btnBack);

    connect(btnLogin, &QPushButton::clicked, this, &AdminLoginWidget::onLoginClicked);
    connect(btnBack, &QPushButton::clicked, this, &AdminLoginWidget::backToLoginRequested);
}

void AdminLoginWidget::onLoginClicked()
{
    QJsonObject data;
    data["username"] = leUsername->text().trimmed();
    data["password"] = lePassword->text();

    QJsonObject response = NetworkClient::instance().sendRequest(RequestType::Login, data);
    if (response.value("status").toString() != "Success") {
        QMessageBox::warning(this, "Access Denied", response.value("message").toString("Incorrect admin credentials."));
        return;
    }

    if (response.value("data").toObject().value("role").toString() != "Admin") {
        QMessageBox::warning(this, "Access Denied", "This account does not have admin access.");
        return;
    }

    lePassword->clear();
    emit adminSignInSuccess();
}

void AdminLoginWidget::paintEvent(QPaintEvent *event)
{
    QStyleOption opt;
    opt.initFrom(this);
    QPainter painter(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &painter, this);
}