#include "forgotpasswordwidget.h"
#include <QVBoxLayout>
#include <QMessageBox>
#include <QPainter>
#include <QStyleOption>

ForgotPasswordWidget::ForgotPasswordWidget(UserManager *manager, QWidget *parent)
    : QWidget(parent), userManager(manager)
{
    this->setObjectName("forgotPage");
    this->setStyleSheet("QWidget#forgotPage { background-color: #FCFCFC; }");

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->setContentsMargins(60, 60, 60, 60);
    layout->setSpacing(15);
    layout->setAlignment(Qt::AlignTop);

    QLabel *title = new QLabel("Reset your password", this);
    title->setFont(QFont("Segoe UI", 20, QFont::Bold));
    title->setStyleSheet("color: #2E4D63;");
    layout->addWidget(title);

    leUsername = new QLineEdit(this);
    leUsername->setPlaceholderText("Enter your username");
    leUsername->setMinimumHeight(38);
    layout->addWidget(leUsername);

    btnFindUser = new QPushButton("Continue", this);
    btnFindUser->setStyleSheet(
        "QPushButton { background-color: rgba(255,255,255,220); color: #2C3E50; border: 1.5px solid #FFC0CB; border-radius: 8px; font-weight: bold; padding: 8px; }"
        "QPushButton:hover { background-color: #FFC0CB; color: white; }"
        );
    layout->addWidget(btnFindUser);

    lblQuestion = new QLabel(this);
    lblQuestion->setStyleSheet("color: #706357;");
    lblQuestion->setWordWrap(true);
    lblQuestion->hide();
    layout->addWidget(lblQuestion);

    leAnswer = new QLineEdit(this);
    leAnswer->setPlaceholderText("Your answer");
    leAnswer->hide();
    layout->addWidget(leAnswer);

    leNewPassword = new QLineEdit(this);
    leNewPassword->setPlaceholderText("New password");
    leNewPassword->setEchoMode(QLineEdit::Password);
    leNewPassword->hide();
    layout->addWidget(leNewPassword);

    leConfirmPassword = new QLineEdit(this);
    leConfirmPassword->setPlaceholderText("Confirm new password");
    leConfirmPassword->setEchoMode(QLineEdit::Password);
    leConfirmPassword->hide();
    layout->addWidget(leConfirmPassword);

    btnReset = new QPushButton("Reset Password", this);
    btnReset->setStyleSheet(
        "QPushButton { background-color: #6F4E37; color: white; border-radius: 8px; font-weight: bold; padding: 10px; }"
        "QPushButton:hover { background-color: #A33A4A; }"
        );
    btnReset->hide();
    layout->addWidget(btnReset);

    btnBack = new QPushButton("Back to Login", this);
    btnBack->setStyleSheet("color: #6F4E37; background: transparent; border: none; font-weight: bold;");
    btnBack->setCursor(Qt::PointingHandCursor);
    layout->addWidget(btnBack);

    connect(btnFindUser, &QPushButton::clicked, this, &ForgotPasswordWidget::onFindUserClicked);
    connect(btnReset, &QPushButton::clicked, this, &ForgotPasswordWidget::onResetClicked);
    connect(btnBack, &QPushButton::clicked, this, &ForgotPasswordWidget::backToLoginRequested);
}

void ForgotPasswordWidget::onFindUserClicked()
{
    QString username = leUsername->text().trimmed();
    if (username.isEmpty()) {
        QMessageBox::warning(this, "Error", "Please enter your username.");
        return;
    }

    // The security answer can only be verified server-side (it's stored
    // encrypted, never sent to the client), so both the answer and the new
    // password are collected here and checked together by ResetPassword.
    currentUsername = username;

    lblQuestion->setText("Security question: What is your favorite book or author?");
    lblQuestion->show();
    leAnswer->show();
    leNewPassword->show();
    leConfirmPassword->show();
    btnReset->show();
}

void ForgotPasswordWidget::onResetClicked()
{
    if (leAnswer->text().trimmed().isEmpty()) {
        QMessageBox::warning(this, "Error", "Please answer the security question.");
        return;
    }

    if (leNewPassword->text().isEmpty() || leNewPassword->text() != leConfirmPassword->text()) {
        QMessageBox::warning(this, "Error", "Passwords do not match.");
        return;
    }

    QString errorMessage;
    bool success = userManager->resetPassword(
        currentUsername, leAnswer->text().trimmed(), leNewPassword->text(), errorMessage);

    if (success) {
        QMessageBox::information(this, "Success", "Your password has been reset. Please log in.");
        emit backToLoginRequested();
    } else {
        QMessageBox::warning(this, "Error", errorMessage.isEmpty() ? "Something went wrong." : errorMessage);
    }
}
void ForgotPasswordWidget::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);

    QStyleOption opt;
    opt.initFrom(this);
    QPainter painter(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &painter, this);
}