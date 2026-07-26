#include "publisherdashboardwidget.h"
#include "addeditbookdialog.h"
#include <QScrollArea>
#include <QHBoxLayout>
#include <QFrame>
#include <QMessageBox>
#include <QInputDialog>
#include <QStyleOption>
#include <QPainter>
#include <algorithm>

PublisherDashboardWidget::PublisherDashboardWidget(PublisherManager *manager, QWidget *parent)
    : QWidget(parent), publisherManager(manager)
{
    this->setObjectName("publisherDashboard");
    this->setStyleSheet("QWidget#publisherDashboard { background-color: #FFF8F2; }");

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

    QLabel *lblPageTitle = new QLabel("Publisher Dashboard", headerWidget);
    lblPageTitle->setFont(QFont("Segoe UI", 18, QFont::Bold));
    lblPageTitle->setStyleSheet("color: #2C3E50; background: transparent;");
    headerLayout->addWidget(lblPageTitle);
    headerLayout->addStretch();

    logoutButton = new QPushButton("Log out", headerWidget);
    logoutButton->setCursor(Qt::PointingHandCursor);
    logoutButton->setStyleSheet(
        "QPushButton { background-color: #FFC0CB; color: #2C3E50; border: none; border-radius: 15px; padding: 8px 16px; font-weight: bold; }"
        "QPushButton:hover { background-color: #FFB6C1; }"
        );
    headerLayout->addWidget(logoutButton);

    outerLayout->addWidget(headerWidget);

    // بدنه اسکرول‌شونده
    QScrollArea *scrollArea = new QScrollArea(this);
    scrollArea->setWidgetResizable(true);
    scrollArea->setFrameShape(QFrame::NoFrame);
    scrollArea->setStyleSheet(
        "QScrollArea { background: transparent; }"
        "QScrollArea > QWidget > QWidget { background: transparent; }"
        );

    QWidget *scrollWidget = new QWidget(scrollArea);
    scrollWidget->setStyleSheet("background: transparent;");
    QVBoxLayout *mainLayout = new QVBoxLayout(scrollWidget);
    mainLayout->setContentsMargins(30, 30, 30, 30);
    mainLayout->setSpacing(20);

    // ===== کارت ۱: اطلاعات حساب =====
    QFrame *infoCard = new QFrame(scrollWidget);
    infoCard->setStyleSheet("background-color: #F2E8DF; border-radius: 20px;");
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
    leUsername->setPlaceholderText("Enter your username");
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

    QFrame *passCard = new QFrame(scrollWidget);
    passCard->setStyleSheet("background-color: rgba(210, 235, 255, 170); border-radius: 20px;");
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
    leNewPassword->setPlaceholderText("New password");
    leNewPassword->setEchoMode(QLineEdit::Password);
    leNewPassword->setMinimumHeight(38);
    leNewPassword->setStyleSheet("background-color: rgba(255,255,255,210); border: 1px solid #FFC0CB; border-radius: 8px; padding: 6px; color: #2C3E50;");
    passLayout->addWidget(leNewPassword);

    leConfirmPassword = new QLineEdit(passCard);
    leConfirmPassword->setPlaceholderText("Confirm new password");
    leConfirmPassword->setEchoMode(QLineEdit::Password);
    leConfirmPassword->setMinimumHeight(38);
    leConfirmPassword->setStyleSheet("background-color: rgba(255,255,255,210); border: 1px solid #FFC0CB; border-radius: 8px; padding: 6px; color: #2C3E50;");
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

    // ===== کارت ۲: مدیریت کتاب‌ها =====
    QFrame *booksCard = new QFrame(scrollWidget);
    booksCard->setStyleSheet("background-color: rgba(210, 235, 255, 170); border-radius: 20px;");
    QVBoxLayout *booksCardLayout = new QVBoxLayout(booksCard);
    booksCardLayout->setContentsMargins(25, 20, 25, 20);
    booksCardLayout->setSpacing(10);

    QHBoxLayout *booksHeader = new QHBoxLayout();
    QLabel *booksTitle = new QLabel("My Books", booksCard);
    booksTitle->setFont(QFont("Segoe UI", 15, QFont::Bold));
    booksTitle->setStyleSheet("color: #2C3E50; background: transparent;");
    booksHeader->addWidget(booksTitle);
    booksHeader->addStretch();

    btnAddBook = new QPushButton("+ Add New Book", booksCard);
    btnAddBook->setCursor(Qt::PointingHandCursor);
    btnAddBook->setStyleSheet(
        "QPushButton { background-color: #2C3E50; color: white; border-radius: 8px; font-weight: bold; padding: 8px 14px; }"
        "QPushButton:hover { background-color: #FFC0CB; color: #2C3E50; }"
        );
    booksHeader->addWidget(btnAddBook);
    booksCardLayout->addLayout(booksHeader);

    bookListLayout = new QVBoxLayout();
    bookListLayout->setSpacing(8);
    booksCardLayout->addLayout(bookListLayout);

    mainLayout->addWidget(booksCard);

    // ===== کارت ۳: داشبورد آمار =====
    QFrame *statsCard = new QFrame(scrollWidget);
    statsCard->setStyleSheet("background-color: rgba(255, 192, 159, 195); border-radius: 20px;");
    QVBoxLayout *statsLayout = new QVBoxLayout(statsCard);
    statsLayout->setContentsMargins(25, 20, 25, 20);
    statsLayout->setSpacing(10);

    QLabel *statsTitle = new QLabel("Statistics", statsCard);
    statsTitle->setFont(QFont("Segoe UI", 15, QFont::Bold));
    statsTitle->setStyleSheet("color: #2C3E50; background: transparent;");
    statsLayout->addWidget(statsTitle);

    lblTotalBooks = new QLabel(statsCard);
    lblTotalBooks->setStyleSheet("color: #2C3E50; background: transparent;");
    statsLayout->addWidget(lblTotalBooks);

    lblTotalRevenue = new QLabel(statsCard);
    lblTotalRevenue->setFont(QFont("Segoe UI", 13, QFont::Bold));
    lblTotalRevenue->setStyleSheet("color: #2C3E50; background: transparent;");
    statsLayout->addWidget(lblTotalRevenue);

    QLabel *bestTitle = new QLabel("Top 5 Best Sellers", statsCard);
    bestTitle->setFont(QFont("Segoe UI", 12, QFont::DemiBold));
    bestTitle->setStyleSheet("color: #2C3E50; background: transparent;");
    statsLayout->addWidget(bestTitle);

    bestSellersLayout = new QVBoxLayout();
    bestSellersLayout->setSpacing(4);
    statsLayout->addLayout(bestSellersLayout);

    QLabel *worstTitle = new QLabel("Bottom 5 Worst Sellers", statsCard);
    worstTitle->setFont(QFont("Segoe UI", 12, QFont::DemiBold));
    worstTitle->setStyleSheet("color: #2C3E50; background: transparent;");
    statsLayout->addWidget(worstTitle);

    worstSellersLayout = new QVBoxLayout();
    worstSellersLayout->setSpacing(4);
    statsLayout->addLayout(worstSellersLayout);

    mainLayout->addWidget(statsCard);
    mainLayout->addStretch();

    scrollArea->setWidget(scrollWidget);
    outerLayout->addWidget(scrollArea);

    connect(btnSaveInfo, &QPushButton::clicked, this, &PublisherDashboardWidget::onSaveInfoClicked);
    connect(btnAddBook, &QPushButton::clicked, this, &PublisherDashboardWidget::onAddBookClicked);
    connect(logoutButton, &QPushButton::clicked, this, &PublisherDashboardWidget::onLogoutClicked);
     connect(btnChangePassword, &QPushButton::clicked, this, &PublisherDashboardWidget::onChangePasswordClicked);
}

QString PublisherDashboardWidget::genreToString(genre g) const
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

void PublisherDashboardWidget::loadPublisher(const Publisher &publisher)
{
    currentPublisher = publisher;
    leUsername->setText(currentPublisher.getUsername());


    refreshBookList();
    refreshStats();
}

void PublisherDashboardWidget::refreshBookList()
{
    QLayoutItem *child;
    while ((child = bookListLayout->takeAt(0)) != nullptr) {
        if (child->widget()) child->widget()->deleteLater();
        delete child;
    }

    QVector<Book> books = currentPublisher.getPublishedBooks();

    if (books.isEmpty()) {
        QLabel *lblNone = new QLabel("You haven't published any books yet.");
        lblNone->setStyleSheet("color: #2C3E50; background: transparent;");
        bookListLayout->addWidget(lblNone);
        return;
    }

    for (const Book &book : books) {
        QFrame *row = new QFrame();
        row->setStyleSheet("background-color: rgba(255,255,255,210); border-radius: 10px;");
        QHBoxLayout *rowLayout = new QHBoxLayout(row);
        rowLayout->setContentsMargins(15, 10, 15, 10);
        rowLayout->setSpacing(10);

        QString statusText = book.getisActive() ? "🟢" : "🔴 (inactive)";
        QLabel *lblBook = new QLabel(QString("%1 %2 — %3 | $%4 | ⭐ %5")
                                         .arg(statusText, book.getTitle(), genreToString(book.getGenre()))
                                         .arg(book.getFinalPrice(), 0, 'f', 2)
                                         .arg(book.getAverageRating(), 0, 'f', 1));
        lblBook->setStyleSheet("color: #2C3E50; background: transparent;");
        rowLayout->addWidget(lblBook, 1);

        QPushButton *btnEdit = new QPushButton("Edit", row);
        btnEdit->setCursor(Qt::PointingHandCursor);
        btnEdit->setStyleSheet(
            "QPushButton { background-color: #FFC0CB; color: #2C3E50; border: none; border-radius: 6px; padding: 4px 10px; }"
            "QPushButton:hover { background-color: #FFB6C1; }"
            );
        rowLayout->addWidget(btnEdit);

        QPushButton *btnDiscount = new QPushButton("Discount", row);
        btnDiscount->setCursor(Qt::PointingHandCursor);
        btnDiscount->setStyleSheet(
            "QPushButton { background-color: #FF69B4; color: white; border: none; border-radius: 6px; padding: 4px 10px; }"
            "QPushButton:hover { background-color: #FFC0CB; color: #2C3E50; }"
            );
        rowLayout->addWidget(btnDiscount);

        QPushButton *btnToggleActive = new QPushButton(book.getisActive() ? "Deactivate" : "Activate", row);
        btnToggleActive->setCursor(Qt::PointingHandCursor);
        btnToggleActive->setStyleSheet(
            "QPushButton { background-color: #2C3E50; color: white; border: none; border-radius: 6px; padding: 4px 10px; }"
            "QPushButton:hover { background-color: #FFC0CB; color: #2C3E50; }"
            );
        rowLayout->addWidget(btnToggleActive);

        QPushButton *btnDeletePermanently = new QPushButton("Delete", row);
        btnDeletePermanently->setCursor(Qt::PointingHandCursor);
        btnDeletePermanently->setStyleSheet(
            "QPushButton { background-color: #FF69B4; color: white; border: none; border-radius: 6px; padding: 4px 10px; }"
            "QPushButton:hover { background-color: #FFC0CB; color: #2C3E50; }"
            );
        rowLayout->addWidget(btnDeletePermanently);

        int bookId = book.getId();

        connect(btnEdit, &QPushButton::clicked, this, [this, book]() {
            AddEditBookDialog dialog(this);
            dialog.loadForEdit(book);
            if (dialog.exec() == QDialog::Accepted) {
                Book updated = dialog.getResultBook();
                currentPublisher.updatePublishedBook(updated);
                emit publisherUpdated(currentPublisher);
                emit catalogChanged();
                refreshBookList();
                refreshStats();
            }
        });

        connect(btnDiscount, &QPushButton::clicked, this, [this, bookId]() {
            bool ok;
            double newDiscount = QInputDialog::getDouble(this, "Set Discount",
                                                         "New discount percentage (0-100):",
                                                         0.0, 0.0, 100.0, 1, &ok);
            if (!ok) return;

            QVector<Book> books = currentPublisher.getPublishedBooks();
            for (Book &b : books) {
                if (b.getId() == bookId) {
                    b.setDiscount(newDiscount);
                    currentPublisher.updatePublishedBook(b);
                    break;
                }
            }
            emit publisherUpdated(currentPublisher);
            emit catalogChanged();
            refreshBookList();
        });

        connect(btnDeletePermanently, &QPushButton::clicked, this, [this, bookId]() {
            QMessageBox::StandardButton reply = QMessageBox::question(
                this, "Confirm Delete",
                "This will permanently delete the book. This cannot be undone. Continue?");
            if (reply == QMessageBox::Yes) {
                currentPublisher.removePublishedBook(bookId);
                emit publisherUpdated(currentPublisher);
                emit catalogChanged();
                refreshBookList();
                refreshStats();
            }
        });

        connect(btnToggleActive, &QPushButton::clicked, this, [this, bookId]() {
            QVector<Book> books = currentPublisher.getPublishedBooks();
            for (Book &b : books) {
                if (b.getId() == bookId) {
                    b.setisActive(!b.getisActive());
                    currentPublisher.updatePublishedBook(b);
                    break;
                }
            }
            emit publisherUpdated(currentPublisher);
            emit catalogChanged();
            refreshBookList();
        });

        bookListLayout->addWidget(row);
    }
}

void PublisherDashboardWidget::refreshStats()
{
    QVector<Book> books = currentPublisher.getPublishedBooks();

    lblTotalBooks->setText(QString("Total published books: %1").arg(books.size()));

    double totalRevenue = 0.0;
    for (const Book &b : books) {
        totalRevenue += b.getSalesCount() * b.getFinalPrice();
    }
    lblTotalRevenue->setText(QString("Total revenue: $%1").arg(totalRevenue, 0, 'f', 2));

    QVector<Book> sortedBySales = books;
    std::sort(sortedBySales.begin(), sortedBySales.end(), [](const Book &a, const Book &b) {
        return a.getSalesCount() > b.getSalesCount();
    });

    QLayoutItem *child;
    while ((child = bestSellersLayout->takeAt(0)) != nullptr) {
        if (child->widget()) child->widget()->deleteLater();
        delete child;
    }
    while ((child = worstSellersLayout->takeAt(0)) != nullptr) {
        if (child->widget()) child->widget()->deleteLater();
        delete child;
    }

    if (sortedBySales.isEmpty()) {
        QLabel *lblNone = new QLabel("No books to display.");
        lblNone->setStyleSheet("color: #2C3E50; background: transparent;");
        bestSellersLayout->addWidget(lblNone);
    } else {
        int bestCount = std::min(5, static_cast<int>(sortedBySales.size()));
        for (int i = 0; i < bestCount; ++i) {
            QLabel *lbl = new QLabel(QString("%1. %2 — %3 sold")
                                         .arg(i + 1)
                                         .arg(sortedBySales[i].getTitle())
                                         .arg(sortedBySales[i].getSalesCount()));
            lbl->setStyleSheet("color: #2C3E50; background: transparent;");
            bestSellersLayout->addWidget(lbl);
        }

        int worstCount = std::min(5, static_cast<int>(sortedBySales.size()));
        for (int i = 0; i < worstCount; ++i) {
            int idx = sortedBySales.size() - 1 - i;
            QLabel *lbl = new QLabel(QString("%1. %2 — %3 sold")
                                         .arg(i + 1)
                                         .arg(sortedBySales[idx].getTitle())
                                         .arg(sortedBySales[idx].getSalesCount()));
            lbl->setStyleSheet("color: #2C3E50; background: transparent;");
            worstSellersLayout->addWidget(lbl);
        }
    }
}

void PublisherDashboardWidget::onSaveInfoClicked()
{
    QString newUsername = leUsername->text().trimmed();

    if (newUsername.isEmpty()) {
        QMessageBox::warning(this, "Error", "Username cannot be empty.");
        return;
    }

    if (publisherManager->isUsernameTakenByAnother(newUsername, currentPublisher.getId())) {
        QMessageBox::warning(this, "Error", "This username is already taken.");
        return;
    }

    currentPublisher.setUsername(newUsername);


    QMessageBox::information(this, "Saved", "Your information has been updated.");
    emit publisherUpdated(currentPublisher);
}
void PublisherDashboardWidget::onChangePasswordClicked()
{
    if (leCurrentPassword->text().isEmpty()) {
        QMessageBox::warning(this, "Error", "Please enter your current password.");
        return;
    }

    if (leCurrentPassword->text() != currentPublisher.getPassword()) {
        QMessageBox::warning(this, "Error", "Current password is incorrect.");
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

    currentPublisher.setPassword(leNewPassword->text());
    leCurrentPassword->clear();
    leNewPassword->clear();
    leConfirmPassword->clear();

    QMessageBox::information(this, "Saved", "Your password has been updated.");
    emit publisherUpdated(currentPublisher);
}
void PublisherDashboardWidget::onAddBookClicked()
{
    AddEditBookDialog dialog(this);
    if (dialog.exec() == QDialog::Accepted) {
        Book newBook = dialog.getResultBook();
        newBook.setPublisherUsername(currentPublisher.getUsername());
        currentPublisher.addPublishedBook(newBook);

        emit publisherUpdated(currentPublisher);
        emit catalogChanged();
        emit bookPublished(newBook);
        refreshBookList();
        refreshStats();
    }
}

void PublisherDashboardWidget::onLogoutClicked()
{
    emit backToLoginRequested();
}

void PublisherDashboardWidget::paintEvent(QPaintEvent *event)
{
    QStyleOption opt;
    opt.initFrom(this);
    QPainter painter(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &painter, this);
}