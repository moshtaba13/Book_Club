#include "profilewidget.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QFrame>
#include <QMessageBox>
#include <QStyleOption>
#include <QPainter>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QCheckBox>
#include <QScrollArea>
#include <QJsonArray>
#include "networkclient.h"
#include "modelserializer.h"


ProfileWidget::ProfileWidget(UserManager *manager, QWidget *parent)
    : QWidget(parent), userManager(manager)
{
    this->setObjectName("profilePage");
    this->setStyleSheet("QWidget#profilePage { background-color: #FFFFFF; }");

    QVBoxLayout *outerLayout = new QVBoxLayout(this);
    outerLayout->setContentsMargins(0, 0, 0, 0);
    outerLayout->setSpacing(0);

    // هدر
    QWidget *headerWidget = new QWidget(this);
    headerWidget->setMinimumHeight(75);
    headerWidget->setMaximumHeight(75);
    headerWidget->setStyleSheet("background-color: rgba(210, 235, 255, 170); border-bottom: 1px solid rgba(0,0,0,15);");
    QHBoxLayout *headerLayout = new QHBoxLayout(headerWidget);
    headerLayout->setContentsMargins(25, 0, 25, 0);

    backButton = new QPushButton("⬅ Back to Club", headerWidget);
    backButton->setCursor(Qt::PointingHandCursor);
    backButton->setStyleSheet(
        "QPushButton {"
        "   background-color: #f4dbde;"
        "   color: #2C3E50;"
        "   border: none;"
        "   border-radius: 15px;"
        "   padding-left: 15px;"
        "   padding-right: 15px;"
        "   min-height: 35px;"
        "   font-weight: bold;"
        "   font-size: 12px;"
        "}"
        "QPushButton:hover { background-color: #FFB6C1; }"
        );
    headerLayout->addWidget(backButton);
    headerLayout->addStretch();

    QLabel *lblPageTitle = new QLabel("My Profile", headerWidget);
    lblPageTitle->setFont(QFont("Segoe UI", 18, QFont::Bold));
    lblPageTitle->setStyleSheet("color: #2C3E50; background: transparent;");
    headerLayout->addWidget(lblPageTitle);
    headerLayout->addStretch();

    outerLayout->addWidget(headerWidget);

    // بدنه اسکرول‌شونده
    QScrollArea *scrollArea = new QScrollArea(this);
    scrollArea->setWidgetResizable(true);
    scrollArea->setFrameShape(QFrame::NoFrame);
    scrollArea->setStyleSheet(
        "QScrollArea { background-color: #FFFFFF; border: none; }"
        "QScrollArea QWidget { background-color: #FFFFFF; }"
        );

    QWidget *scrollWidget = new QWidget(scrollArea);
    scrollWidget->setStyleSheet(
        "background-color: #FFFFFF;"
        );
    QVBoxLayout *mainLayout = new QVBoxLayout(scrollWidget);
    mainLayout->setContentsMargins(30, 30, 30, 30);
    mainLayout->setSpacing(20);

    // ===== کارت ۱: اطلاعات حساب =====
    QFrame *infoCard = new QFrame(scrollWidget);
    infoCard->setStyleSheet("background-color: rgba(253, 246, 238, 180); border-radius: 20px;");
    QVBoxLayout *infoLayout = new QVBoxLayout(infoCard);
    infoLayout->setContentsMargins(25, 20, 25, 20);
    infoLayout->setSpacing(10);

    QLabel *infoTitle = new QLabel("Account Information", infoCard);
    infoTitle->setFont(QFont("Segoe UI", 15, QFont::Bold));
    infoTitle->setStyleSheet("color: #2C3E50; background: transparent;");
    infoLayout->addWidget(infoTitle);

    QLabel *lblUsername = new QLabel("Username", infoCard);
    lblUsername->setStyleSheet("color: #2C3E50; background: transparent; font-size: 11px;");
    infoLayout->addWidget(lblUsername);

    leUsername = new QLineEdit(infoCard);
    leUsername->setMinimumHeight(38);
    leUsername->setStyleSheet("background-color: rgba(255,255,255,220); border: 1px solid #FFC0CB; border-radius: 8px; padding: 6px; color: #2C3E50;");
    infoLayout->addWidget(leUsername);




    btnSaveInfo = new QPushButton("Save Changes", infoCard);
    btnSaveInfo->setCursor(Qt::PointingHandCursor);
    btnSaveInfo->setMinimumHeight(40);
    btnSaveInfo->setStyleSheet(
        "QPushButton { background-color: #FFC0CB; color: #2C3E50; border: none; border-radius: 8px; font-weight: bold; }"
        "QPushButton:hover { background-color: #FFB6C1; }"
        );
    infoLayout->addWidget(btnSaveInfo);

    mainLayout->addWidget(infoCard);

    // ===== کارت ۲: تغییر رمز عبور =====
    QFrame *passCard = new QFrame(scrollWidget);
    passCard->setStyleSheet("background-color: #EAF5FF; border-radius: 20px;");
    QVBoxLayout *passLayout = new QVBoxLayout(passCard);
    passLayout->setContentsMargins(25, 20, 25, 20);
    passLayout->setSpacing(10);

    QLabel *passTitle = new QLabel("Change Password", passCard);
    passTitle->setFont(QFont("Segoe UI", 15, QFont::Bold));
    passTitle->setStyleSheet("color: #2C3E50; background: transparent;");
    passLayout->addWidget(passTitle);

    leCurrentPassword = new QLineEdit(passCard);
    leCurrentPassword->setPlaceholderText("Current password");
    leCurrentPassword->setEchoMode(QLineEdit::Password);
    leCurrentPassword->setMinimumHeight(38);
    leCurrentPassword->setStyleSheet("background-color: rgba(255,255,255,210); border: 1px solid #FFC0CB; border-radius: 8px; padding: 6px; color: #2C3E50;");
    passLayout->addWidget(leCurrentPassword);

    leNewPassword = new QLineEdit(passCard);

    leNewPassword = new QLineEdit(passCard);
    leNewPassword->setPlaceholderText("New password");
    leNewPassword->setEchoMode(QLineEdit::Password);
    leNewPassword->setMinimumHeight(38);
    leNewPassword->setStyleSheet("background-color: rgba(210, 235, 255, 170); border: 1px solid #FFC0CB; border-radius: 8px; padding: 6px; color: #2C3E50;");
    passLayout->addWidget(leNewPassword);

    leConfirmPassword = new QLineEdit(passCard);
    leConfirmPassword->setPlaceholderText("Confirm new password");
    leConfirmPassword->setEchoMode(QLineEdit::Password);
    leConfirmPassword->setMinimumHeight(38);
    leConfirmPassword->setStyleSheet("background-color: rgba(210, 235, 255, 170); border: 1px solid #FFC0CB; border-radius: 8px; padding: 6px; color: #2C3E50;");
    passLayout->addWidget(leConfirmPassword);

    btnChangePassword = new QPushButton("Update Password", passCard);
    btnChangePassword->setCursor(Qt::PointingHandCursor);
    btnChangePassword->setMinimumHeight(40);
    btnChangePassword->setStyleSheet(
        "QPushButton { background-color: #FFC0CB; color: white; border: none; border-radius: 8px; font-weight: bold; }"
        "QPushButton:hover { background-color: #FFC0CB; color: #2C3E50; }"
        );
    passLayout->addWidget(btnChangePassword);

    mainLayout->addWidget(passCard);

    // ===== کارت ۳: ژانرهای مورد علاقه =====
    QFrame *genreCard = new QFrame(scrollWidget);
    genreCard->setStyleSheet("background-color: rgba(253, 246, 238, 180); border-radius: 20px;");
    QVBoxLayout *genreCardLayout = new QVBoxLayout(genreCard);
    genreCardLayout->setContentsMargins(25, 20, 25, 20);
    genreCardLayout->setSpacing(10);

    QLabel *genreTitle = new QLabel("Favorite Genres (1 to 3)", genreCard);
    genreTitle->setFont(QFont("Segoe UI", 15, QFont::Bold));
    genreTitle->setStyleSheet("color: #2C3E50; background: transparent;");
    genreCardLayout->addWidget(genreTitle);

    QGridLayout *genreGrid = new QGridLayout();
    genreGrid->setSpacing(10);

    genreValues = {
        genre::Fiction, genre::Non_fiction, genre::Academic, genre::Drama, genre::Mystery,
        genre::Crime, genre::Romance, genre::History, genre::Science_fiction, genre::Scary
    };

    int row = 0, col = 0;
    for (genre g : genreValues) {
        QCheckBox *chk = new QCheckBox(genreToString(g), genreCard);
        chk->setStyleSheet(
            "QCheckBox { color: #2C3E50; background: transparent; padding: 6px; }"
            "QCheckBox::indicator { width: 18px; height: 18px; border: 2px solid #FFC0CB; border-radius: 4px; background-color: #FFFFFF; }"
            "QCheckBox::indicator:checked { background-color: #FFB6C1; border: 2px solid #FFB6C1; }"
            );
        genreCheckboxes.append(chk);
        genreGrid->addWidget(chk, row, col);
        col++;
        if (col >= 2) { col = 0; row++; }
    }
    genreCardLayout->addLayout(genreGrid);

    btnSaveGenres = new QPushButton("Save Genres", genreCard);
    btnSaveGenres->setCursor(Qt::PointingHandCursor);
    btnSaveGenres->setMinimumHeight(40);
    btnSaveGenres->setStyleSheet(
        "QPushButton { background-color: #FFC0CB; color: #2C3E50; border: none; border-radius: 8px; font-weight: bold; }"
        "QPushButton:hover { background-color: #FFB6C1; }"
        );
    genreCardLayout->addWidget(btnSaveGenres);

    mainLayout->addWidget(genreCard);

    // ===== کارت ۴: تاریخچه خرید =====
    QFrame *historyCard = new QFrame(scrollWidget);
    historyCard->setStyleSheet("background-color: rgba(253, 246, 238, 180); border-radius: 20px;");
    QVBoxLayout *historyLayout = new QVBoxLayout(historyCard);
    historyLayout->setContentsMargins(25, 20, 25, 20);
    historyLayout->setSpacing(10);

    QLabel *historyTitle = new QLabel("Purchase History", historyCard);
    historyTitle->setFont(QFont("Segoe UI", 15, QFont::Bold));
    historyTitle->setStyleSheet("color: #2C3E50; background: transparent;");
    historyLayout->addWidget(historyTitle);

    lblPurchaseCount = new QLabel(historyCard);
    lblPurchaseCount->setStyleSheet("color: #2C3E50; background: transparent; font-size: 12px;");
    historyLayout->addWidget(lblPurchaseCount);

    purchaseHistoryLayout = new QVBoxLayout();
    purchaseHistoryLayout->setSpacing(6);
    historyLayout->addLayout(purchaseHistoryLayout);

    mainLayout->addWidget(historyCard);

    btnLogout = new QPushButton("Log Out", scrollWidget);
    btnLogout->setCursor(Qt::PointingHandCursor);
    btnLogout->setMinimumHeight(42);
    btnLogout->setStyleSheet(
        "QPushButton { background-color: #FFC0CB; color: white; border: none; border-radius: 8px; font-weight: bold; }"
        "QPushButton:hover { background-color: #FFC0CB; color: #2C3E50; }"
        );
    mainLayout->addWidget(btnLogout);

    mainLayout->addStretch();

    scrollArea->setWidget(scrollWidget);
    outerLayout->addWidget(scrollArea);

    connect(backButton, &QPushButton::clicked, this, &ProfileWidget::onBackClicked);
    connect(btnSaveInfo, &QPushButton::clicked, this, &ProfileWidget::onSaveInfoClicked);
    connect(btnChangePassword, &QPushButton::clicked, this, &ProfileWidget::onChangePasswordClicked);
    connect(btnSaveGenres, &QPushButton::clicked, this, &ProfileWidget::onSaveGenresClicked);
    connect(btnLogout, &QPushButton::clicked, this, &ProfileWidget::logoutRequested);
}

QString ProfileWidget::genreToString(genre g) const
{
    switch (g) {
    case genre::Fiction: return "Fiction";
    case genre::Non_fiction: return "Non-Fiction";
    case genre::Academic: return "Academic";
    case genre::Drama: return "Drama";
    case genre::Mystery: return "Mystery";
    case genre::Crime: return "Crime";
    case genre::Romance: return "Romance";
    case genre::History: return "History";
    case genre::Science_fiction: return "Science Fiction";
    case genre::Scary: return "Scary";
    }
    return "Unknown";
}

void ProfileWidget::loadUser(const User &user)
{
    currentUser = user;

    leUsername->setText(currentUser.getUsername());


    refreshGenreCheckboxes();
    refreshPurchaseHistory();
}

void ProfileWidget::refreshGenreCheckboxes()
{
    QVector<genre> favorites = currentUser.getfavoriteGenres();
    for (int i = 0; i < genreCheckboxes.size(); ++i) {
        genreCheckboxes[i]->setChecked(favorites.contains(genreValues[i]));
    }
}

void ProfileWidget::refreshPurchaseHistory()
{
    QLayoutItem *child;
    while ((child = purchaseHistoryLayout->takeAt(0)) != nullptr) {
        if (child->widget()) child->widget()->deleteLater();
        delete child;
    }

    QVector<Book> purchased;
    QJsonObject response = NetworkClient::instance().sendRequest(RequestType::GetPurchasedBooks);
    if (response.value("status").toString() == "Success") {
        for (const QJsonValue &v : response.value("data").toObject().value("purchases").toArray())
            purchased.append(ModelSerializer::purchaseFromJson(v.toObject()).getBook());
    }
    lblPurchaseCount->setText(QString("Total books purchased: %1").arg(purchased.size()));

    if (purchased.isEmpty()) {
        QLabel *lblNone = new QLabel("No purchases yet.");
        lblNone->setStyleSheet("color: #2C3E50; background: transparent;");
        purchaseHistoryLayout->addWidget(lblNone);
        return;
    }

    for (const Book &book : purchased) {
        QLabel *lblItem = new QLabel(QString("📖 %1 — $%2").arg(book.getTitle()).arg(book.getFinalPrice(), 0, 'f', 2));
        lblItem->setStyleSheet("color: #2C3E50; background-color: rgba(255,255,255,210); border-radius: 8px; padding: 8px;");
        purchaseHistoryLayout->addWidget(lblItem);
    }
}

void ProfileWidget::onSaveInfoClicked()
{
    QString newUsername = leUsername->text().trimmed();

    if (newUsername.isEmpty()) {
        QMessageBox::warning(this, "Error", "Username cannot be empty.");
        return;
    }

    QString errorMessage;
    if (!userManager->changeUsername(newUsername, errorMessage)) {
        QMessageBox::warning(this, "Error", errorMessage.isEmpty() ? "Could not update the username." : errorMessage);
        return;
    }

    currentUser.setUsername(newUsername);

    QMessageBox::information(this, "Saved", "Your information has been updated.");
    emit userUpdated(currentUser);
}

void ProfileWidget::onChangePasswordClicked()
{
    if (leCurrentPassword->text().isEmpty()) {
        QMessageBox::warning(this, "Error", "Please enter your current password.");
        return;
    }

    if (leNewPassword->text().isEmpty()) {
        QMessageBox::warning(this, "Error", "Please enter a new password.");
        return;
    }

    if (leNewPassword->text() != leConfirmPassword->text()) {
        QMessageBox::warning(this, "Error", "New passwords do not match.");
        return;
    }

    if (leNewPassword->text() == leCurrentPassword->text()) {
        QMessageBox::warning(this, "Error", "New password must be different from the current password.");
        return;
    }

    QString errorMessage;
    bool success = userManager->changePassword(leCurrentPassword->text(), leNewPassword->text(), errorMessage);

    leCurrentPassword->clear();
    leNewPassword->clear();
    leConfirmPassword->clear();

    if (!success) {
        QMessageBox::warning(this, "Error", errorMessage.isEmpty() ? "Could not update the password." : errorMessage);
        return;
    }

    QMessageBox::information(this, "Saved", "Your password has been updated.");
}

void ProfileWidget::onSaveGenresClicked()
{
    QVector<genre> selected;
    for (int i = 0; i < genreCheckboxes.size(); ++i) {
        if (genreCheckboxes[i]->isChecked()) {
            selected.append(genreValues[i]);
        }
    }

    if (selected.isEmpty() || selected.size() > 3) {
        QMessageBox::warning(this, "Error", "Please select 1 to 3 genres.");
        return;
    }

    QString errorMessage;
    if (!userManager->updateFavoriteGenres(selected, errorMessage)) {
        QMessageBox::warning(this, "Error", errorMessage.isEmpty() ? "Could not update your genres." : errorMessage);
        return;
    }

    currentUser.setFavoriteGenres(selected);

    QMessageBox::information(this, "Saved", "Your favorite genres have been updated.");
    emit userUpdated(currentUser);
}

void ProfileWidget::onBackClicked()
{
    emit backToHomeRequested();
}

void ProfileWidget::paintEvent(QPaintEvent *event)
{
    QStyleOption opt;
    opt.initFrom(this);
    QPainter painter(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &painter, this);
}