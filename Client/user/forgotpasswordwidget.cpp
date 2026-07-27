#include "forgotpasswordwidget.h"
#include <QVBoxLayout>
#include <QMessageBox>
#include <QJsonObject>

ForgotPasswordWidget::ForgotPasswordWidget(QWidget *parent)
    : QWidget(parent)
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

    leAnswer = new QLineEdit(this);
    leAnswer->setPlaceholderText("Your favorite book or author (security answer)");
    leAnswer->setMinimumHeight(38);
    layout->addWidget(leAnswer);

    leNewPassword = new QLineEdit(this);
    leNewPassword->setPlaceholderText("New password");
    leNewPassword->setEchoMode(QLineEdit::Password);
    leNewPassword->setMinimumHeight(38);
    layout->addWidget(leNewPassword);

    leConfirmPassword = new QLineEdit(this);
    leConfirmPassword->setPlaceholderText("Confirm new password");
    leConfirmPassword->setEchoMode(QLineEdit::Password);
    leConfirmPassword->setMinimumHeight(38);
    layout->addWidget(leConfirmPassword);

    btnReset = new QPushButton("Reset Password", this);
    btnReset->setStyleSheet(
        "QPushButton { background-color: #6F4E37; color: white; border-radius: 8px; font-weight: bold; padding: 10px; }"
        "QPushButton:hover { background-color: #A33A4A; }"
        );
    layout->addWidget(btnReset);

    btnBack = new QPushButton("Back to Login", this);
    btnBack->setStyleSheet("color: #6F4E37; background: transparent; border: none; font-weight: bold;");
    btnBack->setCursor(Qt::PointingHandCursor);
    layout->addWidget(btnBack);

    connect(btnReset, &QPushButton::clicked, this, &ForgotPasswordWidget::onResetClicked);
    connect(btnBack, &QPushButton::clicked, this, &ForgotPasswordWidget::backToLoginRequested);
    connect(&NetworkManager::instance(), &NetworkManager::responseReceived,
            this, &ForgotPasswordWidget::onNetworkResponse);
}

void ForgotPasswordWidget::onResetClicked()
{
    if (leUsername->text().trimmed().isEmpty()) {
        QMessageBox::warning(this, "Error", "Please enter your username.");
        return;
    }
    if (leAnswer->text().trimmed().isEmpty()) {
        QMessageBox::warning(this, "Error", "Please answer the security question.");
        return;
    }
    if (leNewPassword->text().isEmpty() || leNewPassword->text() != leConfirmPassword->text()) {
        QMessageBox::warning(this, "Error", "Passwords do not match.");
        return;
    }

    if (!NetworkManager::instance().isConnected()) {
        QMessageBox::warning(this, "Connection Error", "Not connected to the server.");
        return;
    }

    QJsonObject data;
    data["username"] = leUsername->text().trimmed();
    data["security_answer"] = leAnswer->text().trimmed();
    data["new_password"] = leNewPassword->text();

    NetworkManager::instance().sendRequest(RequestType::ResetPassword, data);
}

void ForgotPasswordWidget::onNetworkResponse(RequestType type, ResponseStatus status, const QJsonObject &data, const QString &message)
{
    Q_UNUSED(data);
    if (type != RequestType::ResetPassword) return;

    if (status != ResponseStatus::Success) {
        QMessageBox::warning(this, "Error", message.isEmpty() ? "Something went wrong." : message);
        return;
    }

    QMessageBox::information(this, "Success", "Your password has been reset. Please log in.");
    emit backToLoginRequested();
}