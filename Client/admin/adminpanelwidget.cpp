#include "adminpanelwidget.h"
#include <QScrollArea>
#include <QHBoxLayout>
#include <QFrame>
#include <QLabel>
#include <QMessageBox>
#include <QStyleOption>
#include <QPainter>
#include "networkclient.h"

AdminPanelWidget::AdminPanelWidget(UserManager *userManager, PublisherManager *publisherManager, QWidget *parent)
    : QWidget(parent), userManager(userManager), publisherManager(publisherManager)
{
    this->setObjectName("adminPanel");
    this->setStyleSheet("QWidget#adminPanel { background-color: #FFF8F2; }");

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

    QLabel *lblTitle = new QLabel("Admin Panel", headerWidget);
    lblTitle->setFont(QFont("Segoe UI", 18, QFont::Bold));
    lblTitle->setStyleSheet("color: #2C3E50; background: transparent;");
    headerLayout->addWidget(lblTitle);
    headerLayout->addStretch();

    leSearch = new QLineEdit(headerWidget);
    leSearch->setPlaceholderText("Search users/publishers by username...");
    leSearch->setFixedWidth(280);
    leSearch->setMinimumHeight(36);
    leSearch->setStyleSheet("background-color: rgba(255,255,255,210); border: 1px solid #FFC0CB; border-radius: 18px; padding: 4px 12px; color: #2C3E50;");
    headerLayout->addWidget(leSearch);

    QPushButton *btnLogout = new QPushButton("Log out", headerWidget);
    btnLogout->setCursor(Qt::PointingHandCursor);
    btnLogout->setStyleSheet(
        "QPushButton { background-color: #FFC0CB; color: #2C3E50; border: none; border-radius: 15px; padding: 8px 16px; font-weight: bold; }"
        "QPushButton:hover { background-color: #FFB6C1; }"
        );
    headerLayout->addWidget(btnLogout);

    outerLayout->addWidget(headerWidget);

    // تب‌ها
    QTabWidget *tabs = new QTabWidget(this);
    tabs->setStyleSheet(
        "QTabWidget::pane { border: none; }"
        "QTabBar::tab { background-color: #F2E8DF; color: #2C3E50; padding: 10px 20px; border-top-left-radius: 8px; border-top-right-radius: 8px; }"
        "QTabBar::tab:selected { background-color: rgba(210, 235, 255, 170); font-weight: bold; }"
        );

    // ===== تب کاربران عادی =====
    QScrollArea *usersScroll = new QScrollArea();
    usersScroll->setWidgetResizable(true);
    usersScroll->setFrameShape(QFrame::NoFrame);
    usersScroll->setStyleSheet("background: transparent;");

    QWidget *usersScrollWidget = new QWidget();
    usersScrollWidget->setStyleSheet("background: transparent;");
    QVBoxLayout *usersOuterLayout = new QVBoxLayout(usersScrollWidget);
    usersOuterLayout->setContentsMargins(20, 20, 20, 20);
    usersListLayout = new QVBoxLayout();
    usersListLayout->setSpacing(10);
    usersOuterLayout->addLayout(usersListLayout);
    usersOuterLayout->addStretch();
    usersScroll->setWidget(usersScrollWidget);
    tabs->addTab(usersScroll, "Users");

    // ===== تب ناشران =====
    QScrollArea *publishersScroll = new QScrollArea();
    publishersScroll->setWidgetResizable(true);
    publishersScroll->setFrameShape(QFrame::NoFrame);
    publishersScroll->setStyleSheet("background: transparent;");

    QWidget *publishersScrollWidget = new QWidget();
    publishersScrollWidget->setStyleSheet("background: transparent;");
    QVBoxLayout *publishersOuterLayout = new QVBoxLayout(publishersScrollWidget);
    publishersOuterLayout->setContentsMargins(20, 20, 20, 20);
    publishersListLayout = new QVBoxLayout();
    publishersListLayout->setSpacing(10);
    publishersOuterLayout->addLayout(publishersListLayout);
    publishersOuterLayout->addStretch();
    publishersScroll->setWidget(publishersScrollWidget);
    tabs->addTab(publishersScroll, "Publishers");

    // ===== تب کتاب‌ها و محتوا =====
    QScrollArea *booksScroll = new QScrollArea();
    booksScroll->setWidgetResizable(true);
    booksScroll->setFrameShape(QFrame::NoFrame);
    booksScroll->setStyleSheet("background: transparent;");

    QWidget *booksScrollWidget = new QWidget();
    booksScrollWidget->setStyleSheet("background: transparent;");
    QVBoxLayout *booksOuterLayout = new QVBoxLayout(booksScrollWidget);
    booksOuterLayout->setContentsMargins(20, 20, 20, 20);
    booksListLayout = new QVBoxLayout();
    booksListLayout->setSpacing(10);
    booksOuterLayout->addLayout(booksListLayout);
    booksOuterLayout->addStretch();
    booksScroll->setWidget(booksScrollWidget);
    tabs->addTab(booksScroll, "Books & Reviews");

    outerLayout->addWidget(tabs);

    connect(leSearch, &QLineEdit::textChanged, this, &AdminPanelWidget::onSearchTextChanged);
    connect(btnLogout, &QPushButton::clicked, this, &AdminPanelWidget::logoutRequested);
}

void AdminPanelWidget::refreshAll()
{
    refreshUsersList();
    refreshPublishersList();
    refreshBooksList();
}

void AdminPanelWidget::onSearchTextChanged(const QString &text)
{
    Q_UNUSED(text);
    refreshUsersList();
    refreshPublishersList();
    refreshBooksList();
}

void AdminPanelWidget::refreshUsersList()
{
    QLayoutItem *child;
    while ((child = usersListLayout->takeAt(0)) != nullptr) {
        if (child->widget()) child->widget()->deleteLater();
        delete child;
    }

    QString filter = leSearch->text().trimmed();
    QString errorMessage;
    QVector<User> users = userManager->getAllUsers(errorMessage);
    if (!errorMessage.isEmpty())
        QMessageBox::warning(this, "Error", errorMessage);

    if (users.isEmpty()) {
        QLabel *lblNone = new QLabel("No users registered yet.");
        lblNone->setStyleSheet("color: #2C3E50; background: transparent;");
        usersListLayout->addWidget(lblNone);
        return;
    }

    bool anyShown = false;
    for (const User &u : users) {
        if (!filter.isEmpty() && !u.getUsername().contains(filter, Qt::CaseInsensitive)) {
            continue;
        }
        anyShown = true;

        QFrame *row = new QFrame();
        row->setStyleSheet(u.isBlocked()
                               ? "background-color: rgba(255,105,180,80); border-radius: 10px;"
                               : "background-color: rgba(255,255,255,210); border-radius: 10px;");
        QHBoxLayout *rowLayout = new QHBoxLayout(row);
        rowLayout->setContentsMargins(15, 10, 15, 10);
        rowLayout->setSpacing(10);

        QString statusText = u.isBlocked() ? "🔴 Blocked" : "🟢 Active";
        QLabel *lblInfo = new QLabel(QString("%1 — %2 | Joined: %3")
                                         .arg(u.getUsername(), statusText, u.getRegisterDate().toString("yyyy-MM-dd")));
        lblInfo->setStyleSheet("color: #2C3E50; background: transparent;");
        rowLayout->addWidget(lblInfo, 1);

        int userId = u.getId();
        bool currentlyBlocked = u.isBlocked();

        QPushButton *btnToggleBlock = new QPushButton(currentlyBlocked ? "Unblock" : "Block", row);
        btnToggleBlock->setCursor(Qt::PointingHandCursor);
        btnToggleBlock->setStyleSheet(
            "QPushButton { background-color: #2C3E50; color: white; border: none; border-radius: 6px; padding: 4px 10px; }"
            "QPushButton:hover { background-color: #FFC0CB; color: #2C3E50; }"
            );
        rowLayout->addWidget(btnToggleBlock);

        QPushButton *btnDelete = new QPushButton("Delete", row);
        btnDelete->setCursor(Qt::PointingHandCursor);
        btnDelete->setStyleSheet(
            "QPushButton { background-color: #FF69B4; color: white; border: none; border-radius: 6px; padding: 4px 10px; }"
            "QPushButton:hover { background-color: #FFC0CB; color: #2C3E50; }"
            );
        rowLayout->addWidget(btnDelete);

        connect(btnToggleBlock, &QPushButton::clicked, this, [this, userId, currentlyBlocked]() {
            QString errorMessage;
            if (!userManager->setUserBlocked(userId, !currentlyBlocked, errorMessage))
                QMessageBox::warning(this, "Error", errorMessage.isEmpty() ? "Could not update the user's status." : errorMessage);
            refreshUsersList();
        });

        connect(btnDelete, &QPushButton::clicked, this, [this, userId]() {
            QMessageBox::StandardButton reply = QMessageBox::question(
                this, "Confirm Delete", "Are you sure you want to delete this user account?");
            if (reply == QMessageBox::Yes) {
                QString errorMessage;
                if (!userManager->deleteUser(userId, errorMessage))
                    QMessageBox::warning(this, "Error", errorMessage.isEmpty() ? "Could not delete the user." : errorMessage);
                refreshUsersList();
            }
        });

        usersListLayout->addWidget(row);
    }

    if (!anyShown) {
        QLabel *lblNone = new QLabel("No users match your search.");
        lblNone->setStyleSheet("color: #2C3E50; background: transparent;");
        usersListLayout->addWidget(lblNone);
    }
}

void AdminPanelWidget::refreshPublishersList()
{
    QLayoutItem *child;
    while ((child = publishersListLayout->takeAt(0)) != nullptr) {
        if (child->widget()) child->widget()->deleteLater();
        delete child;
    }

    QString filter = leSearch->text().trimmed();
    QString errorMessage;
    QVector<Publisher> publishers = publisherManager->getAllPublishers(errorMessage);
    if (!errorMessage.isEmpty())
        QMessageBox::warning(this, "Error", errorMessage);

    if (publishers.isEmpty()) {
        QLabel *lblNone = new QLabel("No publishers registered yet.");
        lblNone->setStyleSheet("color: #2C3E50; background: transparent;");
        publishersListLayout->addWidget(lblNone);
        return;
    }

    bool anyShown = false;
    for (const Publisher &p : publishers) {
        if (!filter.isEmpty() && !p.getUsername().contains(filter, Qt::CaseInsensitive)) {
            continue;
        }
        anyShown = true;

        QFrame *row = new QFrame();
        row->setStyleSheet(p.isBlocked()
                               ? "background-color: rgba(255,105,180,80); border-radius: 10px;"
                               : "background-color: rgba(255,255,255,210); border-radius: 10px;");
        QHBoxLayout *rowLayout = new QHBoxLayout(row);
        rowLayout->setContentsMargins(15, 10, 15, 10);
        rowLayout->setSpacing(10);

        QString statusText = p.isBlocked() ? "🔴 Blocked" : "🟢 Active";
        QLabel *lblInfo = new QLabel(QString("%1 — %2 | Books: %3 | Revenue: $%4 | Joined: %5")
                                         .arg(p.getUsername(), statusText)
                                         .arg(p.getPublishedBooks().size())
                                         .arg(p.getTotalRevenue(), 0, 'f', 2)
                                         .arg(p.getRegisterDate().toString("yyyy-MM-dd")));
        lblInfo->setStyleSheet("color: #2C3E50; background: transparent;");
        lblInfo->setWordWrap(true);
        rowLayout->addWidget(lblInfo, 1);

        int publisherId = p.getId();
        bool currentlyBlocked = p.isBlocked();

        QPushButton *btnToggleBlock = new QPushButton(currentlyBlocked ? "Unblock" : "Block", row);
        btnToggleBlock->setCursor(Qt::PointingHandCursor);
        btnToggleBlock->setStyleSheet(
            "QPushButton { background-color: #2C3E50; color: white; border: none; border-radius: 6px; padding: 4px 10px; }"
            "QPushButton:hover { background-color: #FFC0CB; color: #2C3E50; }"
            );
        rowLayout->addWidget(btnToggleBlock);

        QPushButton *btnDelete = new QPushButton("Delete", row);
        btnDelete->setCursor(Qt::PointingHandCursor);
        btnDelete->setStyleSheet(
            "QPushButton { background-color: #FF69B4; color: white; border: none; border-radius: 6px; padding: 4px 10px; }"
            "QPushButton:hover { background-color: #FFC0CB; color: #2C3E50; }"
            );
        rowLayout->addWidget(btnDelete);

        connect(btnToggleBlock, &QPushButton::clicked, this, [this, publisherId, currentlyBlocked]() {
            QString errorMessage;
            if (!publisherManager->setPublisherBlocked(publisherId, !currentlyBlocked, errorMessage))
                QMessageBox::warning(this, "Error", errorMessage.isEmpty() ? "Could not update the publisher's status." : errorMessage);
            refreshPublishersList();
        });

        connect(btnDelete, &QPushButton::clicked, this, [this, publisherId]() {
            QMessageBox::StandardButton reply = QMessageBox::question(
                this, "Confirm Delete", "Are you sure you want to delete this publisher account?");
            if (reply == QMessageBox::Yes) {
                QString errorMessage;
                if (!publisherManager->deletePublisher(publisherId, errorMessage))
                    QMessageBox::warning(this, "Error", errorMessage.isEmpty() ? "Could not delete the publisher." : errorMessage);
                refreshPublishersList();
            }
        });

        publishersListLayout->addWidget(row);
    }

    if (!anyShown) {
        QLabel *lblNone = new QLabel("No publishers match your search.");
        lblNone->setStyleSheet("color: #2C3E50; background: transparent;");
        publishersListLayout->addWidget(lblNone);
    }
}

void AdminPanelWidget::refreshBooksList()
{
    QLayoutItem *child;
    while ((child = booksListLayout->takeAt(0)) != nullptr) {
        if (child->widget()) child->widget()->deleteLater();
        delete child;
    }

    QString filter = leSearch->text().trimmed();
    QString errorMessage;
    QVector<Book> books = publisherManager->getAllBooksAdmin(errorMessage);
    if (!errorMessage.isEmpty())
        QMessageBox::warning(this, "Error", errorMessage);

    if (books.isEmpty()) {
        QLabel *lblNone = new QLabel("No books in the system yet.");
        lblNone->setStyleSheet("color: #2C3E50; background: transparent;");
        booksListLayout->addWidget(lblNone);
        return;
    }

    bool anyShown = false;
    for (const Book &book : books) {
        if (!filter.isEmpty() &&
            !book.getTitle().contains(filter, Qt::CaseInsensitive) &&
            !book.getPublisherUsername().contains(filter, Qt::CaseInsensitive)) {
            continue;
        }
        anyShown = true;

        QFrame *bookBox = new QFrame();
        bookBox->setStyleSheet("background-color: rgba(255,255,255,210); border-radius: 10px;");
        QVBoxLayout *bookBoxLayout = new QVBoxLayout(bookBox);
        bookBoxLayout->setContentsMargins(15, 12, 15, 12);
        bookBoxLayout->setSpacing(8);

        QHBoxLayout *bookHeader = new QHBoxLayout();
        QString statusText = book.getisActive() ? "🟢" : "🔴";
        QLabel *lblInfo = new QLabel(QString("%1 %2 — by %3 | Publisher: %4 | ⭐ %5")
                                         .arg(statusText, book.getTitle(), book.getAuthor(), book.getPublisherUsername())
                                         .arg(book.getAverageRating(), 0, 'f', 1));
        lblInfo->setStyleSheet("color: #2C3E50; background: transparent;");
        lblInfo->setWordWrap(true);
        bookHeader->addWidget(lblInfo, 1);

        int bookId = book.getId();

        QPushButton *btnDeleteBook = new QPushButton("Delete Book", bookBox);
        btnDeleteBook->setCursor(Qt::PointingHandCursor);
        btnDeleteBook->setStyleSheet(
            "QPushButton { background-color: #FF69B4; color: white; border: none; border-radius: 6px; padding: 4px 10px; }"
            "QPushButton:hover { background-color: #FFC0CB; color: #2C3E50; }"
            );
        bookHeader->addWidget(btnDeleteBook);

        connect(btnDeleteBook, &QPushButton::clicked, this, [this, bookId]() {
            QMessageBox::StandardButton reply = QMessageBox::question(
                this, "Confirm Delete", "Permanently delete this book from the system?");
            if (reply == QMessageBox::Yes) {
                QString errorMessage;
                if (!publisherManager->removeBookGlobally(bookId, errorMessage))
                    QMessageBox::warning(this, "Error", errorMessage.isEmpty() ? "Could not delete the book." : errorMessage);
                emit catalogChanged();
                refreshBooksList();
            }
        });

        bookBoxLayout->addLayout(bookHeader);

        // نمایش نظرات و امکان حذف هر نظر توسط ادمین
        if (!book.getReviews().isEmpty()) {
            QLabel *lblReviewsTitle = new QLabel("Reviews:", bookBox);
            lblReviewsTitle->setStyleSheet("color: #706357; font-size: 11px; background: transparent;");
            bookBoxLayout->addWidget(lblReviewsTitle);

            for (const Review &r : book.getReviews()) {
                QHBoxLayout *reviewRow = new QHBoxLayout();
                QLabel *lblReview = new QLabel(QString("  %1 ★ — %2 (%3)").arg(r.getStars()).arg(r.getComment(), r.getUsername()));
                lblReview->setStyleSheet("color: #2C3E50; font-size: 11px; background: transparent;");
                lblReview->setWordWrap(true);
                reviewRow->addWidget(lblReview, 1);

                QPushButton *btnDeleteReview = new QPushButton("Delete Review", bookBox);
                btnDeleteReview->setCursor(Qt::PointingHandCursor);
                btnDeleteReview->setStyleSheet(
                    "QPushButton { background-color: #FF69B4; color: white; border: none; border-radius: 6px; padding: 2px 8px; font-size: 10px; }"
                    "QPushButton:hover { background-color: #FFC0CB; color: #2C3E50; }"
                    );
                reviewRow->addWidget(btnDeleteReview);

                int reviewUserId = r.getUserId();
                connect(btnDeleteReview, &QPushButton::clicked, this, [this, bookId, reviewUserId]() {
                    QString errorMessage;
                    if (!publisherManager->deleteReviewAdmin(bookId, reviewUserId, errorMessage))
                        QMessageBox::warning(this, "Error", errorMessage.isEmpty() ? "Could not delete the review." : errorMessage);
                    refreshBooksList();
                });

                bookBoxLayout->addLayout(reviewRow);
            }
        }

        booksListLayout->addWidget(bookBox);
    }

    if (!anyShown) {
        QLabel *lblNone = new QLabel("No books match your search.");
        lblNone->setStyleSheet("color: #2C3E50; background: transparent;");
        booksListLayout->addWidget(lblNone);
    }
}

void AdminPanelWidget::paintEvent(QPaintEvent *event)
{
    QStyleOption opt;
    opt.initFrom(this);
    QPainter painter(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &painter, this);
}