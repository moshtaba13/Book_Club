#include "personallibrarywidget.h"
#include <QScrollArea>
#include <QHBoxLayout>
#include <QFrame>
#include <QMessageBox>
#include <QStyleOption>
#include <QPainter>
#include <QInputDialog>

PersonalLibraryWidget::PersonalLibraryWidget(QWidget *parent)
    : QWidget(parent), currentUser(nullptr)
{
    this->setObjectName("libraryPage");
    this->setStyleSheet("QWidget#libraryPage { background-color: #FFF8F2; }");

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

    QLabel *lblPageTitle = new QLabel("My Library", headerWidget);
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
        "QScrollArea { background: transparent; }"
        "QScrollArea > QWidget > QWidget { background: transparent; }"
        );

    QWidget *scrollWidget = new QWidget(scrollArea);
    scrollWidget->setStyleSheet("background: transparent;");
    QVBoxLayout *mainLayout = new QVBoxLayout(scrollWidget);
    mainLayout->setContentsMargins(30, 30, 30, 30);
    mainLayout->setSpacing(20);

    // ===== کارت ۱: کتاب‌های من (هلویی) =====
    QFrame *myBooksCard = new QFrame(scrollWidget);
    myBooksCard->setStyleSheet("background-color: rgba(255, 192, 159, 195); border-radius: 20px;");
    QVBoxLayout *myBooksCardLayout = new QVBoxLayout(myBooksCard);
    myBooksCardLayout->setContentsMargins(25, 20, 25, 20);
    myBooksCardLayout->setSpacing(10);

    QLabel *myBooksTitle = new QLabel("My Books", myBooksCard);
    myBooksTitle->setFont(QFont("Segoe UI", 15, QFont::Bold));
    myBooksTitle->setStyleSheet("color: #2C3E50; background: transparent;");
    myBooksCardLayout->addWidget(myBooksTitle);

    myBooksLayout = new QVBoxLayout();
    myBooksLayout->setSpacing(8);
    myBooksCardLayout->addLayout(myBooksLayout);

    mainLayout->addWidget(myBooksCard);

    // ===== کارت ۲: کتاب‌های ذخیره‌شده (هلویی) =====
    QFrame *savedBooksCard = new QFrame(scrollWidget);
    savedBooksCard->setStyleSheet("background-color: rgba(255, 192, 159, 195); border-radius: 20px;");
    QVBoxLayout *savedBooksCardLayout = new QVBoxLayout(savedBooksCard);
    savedBooksCardLayout->setContentsMargins(25, 20, 25, 20);
    savedBooksCardLayout->setSpacing(10);

    QLabel *savedBooksTitle = new QLabel("Saved for Later", savedBooksCard);
    savedBooksTitle->setFont(QFont("Segoe UI", 15, QFont::Bold));
    savedBooksTitle->setStyleSheet("color: #2C3E50; background: transparent;");
    savedBooksCardLayout->addWidget(savedBooksTitle);

    savedBooksLayout = new QVBoxLayout();
    savedBooksLayout->setSpacing(8);
    savedBooksCardLayout->addLayout(savedBooksLayout);

    mainLayout->addWidget(savedBooksCard);

    // ===== کارت ۳: قفسه‌ها (هلویی) =====
    QFrame *shelvesCard = new QFrame(scrollWidget);
    shelvesCard->setStyleSheet("background-color: rgba(255, 192, 159, 195); border-radius: 20px;");
    QVBoxLayout *shelvesCardLayout = new QVBoxLayout(shelvesCard);
    shelvesCardLayout->setContentsMargins(25, 20, 25, 20);
    shelvesCardLayout->setSpacing(10);

    QLabel *shelvesTitle = new QLabel("My Shelves", shelvesCard);
    shelvesTitle->setFont(QFont("Segoe UI", 15, QFont::Bold));
    shelvesTitle->setStyleSheet("color: #2C3E50; background: transparent;");
    shelvesCardLayout->addWidget(shelvesTitle);

    QHBoxLayout *newShelfRow = new QHBoxLayout();
    leNewShelfName = new QLineEdit(shelvesCard);
    leNewShelfName->setPlaceholderText("New shelf name");
    leNewShelfName->setMinimumHeight(36);
    leNewShelfName->setStyleSheet("background-color: rgba(255,255,255,220); border: 1px solid #FFC0CB; border-radius: 8px; padding: 6px; color: #2C3E50;");
    newShelfRow->addWidget(leNewShelfName, 1);

    btnCreateShelf = new QPushButton("+ Create", shelvesCard);
    btnCreateShelf->setCursor(Qt::PointingHandCursor);
    btnCreateShelf->setStyleSheet(
        "QPushButton { background-color: #FFC0CB; color: #2C3E50; border: none; border-radius: 8px; font-weight: bold; padding: 6px 14px; }"
        "QPushButton:hover { background-color: #FFB6C1; }"
        );
    newShelfRow->addWidget(btnCreateShelf);
    shelvesCardLayout->addLayout(newShelfRow);

    shelvesLayout = new QVBoxLayout();
    shelvesLayout->setSpacing(10);
    shelvesCardLayout->addLayout(shelvesLayout);

    mainLayout->addWidget(shelvesCard);
    mainLayout->addStretch();

    scrollArea->setWidget(scrollWidget);
    outerLayout->addWidget(scrollArea);

    connect(backButton, &QPushButton::clicked, this, &PersonalLibraryWidget::onBackClicked);
    connect(btnCreateShelf, &QPushButton::clicked, this, &PersonalLibraryWidget::onCreateShelfClicked);
}

void PersonalLibraryWidget::loadUser(User &user)
{
    currentUser = &user;
    refreshMyBooks();
    refreshSavedBooks();
    refreshShelves();
}

void PersonalLibraryWidget::refreshMyBooks()
{
    QLayoutItem *child;
    while ((child = myBooksLayout->takeAt(0)) != nullptr) {
        if (child->widget()) child->widget()->deleteLater();
        delete child;
    }

    if (!currentUser) return;

    QVector<Book> books;
    for (const Purchase &p : currentUser->getpurchasedBooks()) {
        books.append(p.getBook());
    }
    if (books.isEmpty()) {
        QLabel *lblNone = new QLabel("You haven't purchased any books yet.");
        lblNone->setStyleSheet("color: #2C3E50; background: transparent;");
        myBooksLayout->addWidget(lblNone);
        return;
    }

    for (const Book &book : books) {
        QFrame *row = new QFrame();
        row->setStyleSheet("background-color: rgba(255,255,255,210); border-radius: 10px;");
        QHBoxLayout *rowLayout = new QHBoxLayout(row);
        rowLayout->setContentsMargins(12, 8, 12, 8);

        QLabel *lblBook = new QLabel(QString("📖 %1 — by %2").arg(book.getTitle(), book.getAuthor()));
        lblBook->setStyleSheet("color: #2C3E50; background: transparent;");
        rowLayout->addWidget(lblBook, 1);

        QComboBox *shelfCombo = buildShelfComboBox();
        shelfCombo->setStyleSheet("background-color: white; border-radius: 6px; padding: 4px; color: #2C3E50;");
        rowLayout->addWidget(shelfCombo);

        QPushButton *btnAddToShelf = new QPushButton("Add to Shelf", row);
        btnAddToShelf->setCursor(Qt::PointingHandCursor);
        btnAddToShelf->setStyleSheet(
            "QPushButton { background-color: #FFC0CB; color: #2C3E50; border: none; border-radius: 6px; padding: 4px 10px; }"
            "QPushButton:hover { background-color: #FFB6C1; }"
            );
        rowLayout->addWidget(btnAddToShelf);

        connect(btnAddToShelf, &QPushButton::clicked, this, [this, book, shelfCombo]() {
            if (!currentUser) return;
            QString shelfName = shelfCombo->currentText();
            Shelf *shelf = currentUser->findShelf(shelfName);
            if (shelf) {
                shelf->addBook(book);
                emit userUpdated(*currentUser);
                refreshShelves();
                QMessageBox::information(this, "Added", "Book added to shelf: " + shelfName);
            }
        });

        myBooksLayout->addWidget(row);
    }
}

void PersonalLibraryWidget::refreshSavedBooks()
{
    QLayoutItem *child;
    while ((child = savedBooksLayout->takeAt(0)) != nullptr) {
        if (child->widget()) child->widget()->deleteLater();
        delete child;
    }

    if (!currentUser) return;

    QVector<Book> books = currentUser->getSavedBooks();
    if (books.isEmpty()) {
        QLabel *lblNone = new QLabel("No saved books yet.");
        lblNone->setStyleSheet("color: #2C3E50; background: transparent;");
        savedBooksLayout->addWidget(lblNone);
        return;
    }

    for (const Book &book : books) {
        QFrame *row = new QFrame();
        row->setStyleSheet("background-color: rgba(255,255,255,210); border-radius: 10px;");
        QHBoxLayout *rowLayout = new QHBoxLayout(row);
        rowLayout->setContentsMargins(12, 8, 12, 8);

        QLabel *lblBook = new QLabel(QString("🔖 %1 — by %2").arg(book.getTitle(), book.getAuthor()));
        lblBook->setStyleSheet("color: #2C3E50; background: transparent;");
        rowLayout->addWidget(lblBook, 1);

        QPushButton *btnRemove = new QPushButton("Remove", row);
        btnRemove->setCursor(Qt::PointingHandCursor);
        btnRemove->setStyleSheet(
            "QPushButton { background-color: #FF69B4; color: white; border: none; border-radius: 6px; padding: 4px 10px; }"
            "QPushButton:hover { background-color: #FFC0CB; color: #2C3E50; }"
            );
        rowLayout->addWidget(btnRemove);

        connect(btnRemove, &QPushButton::clicked, this, [this, book]() {
            if (!currentUser) return;
            currentUser->removeSavedBook(book.getId());
            emit userUpdated(*currentUser);
            refreshSavedBooks();
        });

        savedBooksLayout->addWidget(row);
    }
}

QComboBox* PersonalLibraryWidget::buildShelfComboBox(int excludeIndex)
{
    QComboBox *combo = new QComboBox();
    if (currentUser) {
        QVector<Shelf> shelves = currentUser->getShelves();
        for (int i = 0; i < shelves.size(); ++i) {
            if (i == excludeIndex) continue;
            combo->addItem(shelves[i].getName());
        }
    }
    return combo;
}

void PersonalLibraryWidget::refreshShelves()
{
    QLayoutItem *child;
    while ((child = shelvesLayout->takeAt(0)) != nullptr) {
        if (child->widget()) child->widget()->deleteLater();
        delete child;
    }

    if (!currentUser) return;

    QVector<Shelf> shelves = currentUser->getShelves();
    if (shelves.isEmpty()) {
        QLabel *lblNone = new QLabel("No shelves yet. Create one above!");
        lblNone->setStyleSheet("color: #2C3E50; background: transparent;");
        shelvesLayout->addWidget(lblNone);
        return;
    }

    for (const Shelf &shelf : shelves) {
        QFrame *shelfBox = new QFrame();
        shelfBox->setStyleSheet("background-color: rgba(255,255,255,210); border-radius: 10px;");
        QVBoxLayout *shelfBoxLayout = new QVBoxLayout(shelfBox);
        shelfBoxLayout->setContentsMargins(12, 10, 12, 10);
        shelfBoxLayout->setSpacing(6);

        QString shelfName = shelf.getName();

        QHBoxLayout *shelfHeader = new QHBoxLayout();
        QLabel *lblShelfName = new QLabel(QString("📚 %1 (%2 books)").arg(shelfName).arg(shelf.bookCount()));
        lblShelfName->setStyleSheet("color: #2C3E50; font-weight: bold; background: transparent;");
        shelfHeader->addWidget(lblShelfName, 1);

        QPushButton *btnRename = new QPushButton("Rename", shelfBox);
        btnRename->setCursor(Qt::PointingHandCursor);
        btnRename->setStyleSheet(
            "QPushButton { background-color: #FFC0CB; color: #2C3E50; border: none; border-radius: 6px; padding: 4px 10px; }"
            "QPushButton:hover { background-color: #FFB6C1; }"
            );
        shelfHeader->addWidget(btnRename);

        QPushButton *btnDeleteShelf = new QPushButton("Delete", shelfBox);
        btnDeleteShelf->setCursor(Qt::PointingHandCursor);
        btnDeleteShelf->setStyleSheet(
            "QPushButton { background-color: #FF69B4; color: white; border: none; border-radius: 6px; padding: 4px 10px; }"
            "QPushButton:hover { background-color: #FFC0CB; color: #2C3E50; }"
            );
        shelfHeader->addWidget(btnDeleteShelf);
        shelfBoxLayout->addLayout(shelfHeader);

        connect(btnRename, &QPushButton::clicked, this, [this, shelfName]() {
            if (!currentUser) return;
            bool ok;
            QString newName = QInputDialog::getText(this, "Rename Shelf", "New shelf name:", QLineEdit::Normal, shelfName, &ok);
            if (!ok || newName.trimmed().isEmpty()) return;

            for (const Shelf &s : currentUser->getShelves()) {
                if (s.getName() == newName.trimmed()) {
                    QMessageBox::warning(this, "Error", "A shelf with this name already exists.");
                    return;
                }
            }

            currentUser->renameShelf(shelfName, newName.trimmed());
            emit userUpdated(*currentUser);
            refreshShelves();
        });

        connect(btnDeleteShelf, &QPushButton::clicked, this, [this, shelfName]() {
            if (!currentUser) return;
            currentUser->removeShelf(shelfName);
            emit userUpdated(*currentUser);
            refreshShelves();
        });

        for (const Book &book : shelf.getBooks()) {
            QHBoxLayout *bookRow = new QHBoxLayout();

            QLabel *lblBook = new QLabel("  • " + book.getTitle());
            lblBook->setStyleSheet("color: #2C3E50; background: transparent;");
            bookRow->addWidget(lblBook, 1);

            QComboBox *moveCombo = buildShelfComboBox();
            for (int i = 0; i < moveCombo->count(); ++i) {
                if (moveCombo->itemText(i) == shelfName) {
                    moveCombo->removeItem(i);
                    break;
                }
            }
            moveCombo->setStyleSheet("background-color: white; border-radius: 6px; padding: 2px; color: #2C3E50; font-size: 11px;");
            bookRow->addWidget(moveCombo);

            QPushButton *btnMove = new QPushButton("Move", nullptr);
            btnMove->setCursor(Qt::PointingHandCursor);
            btnMove->setStyleSheet(
                "QPushButton { background-color: #2C3E50; color: white; border: none; border-radius: 6px; padding: 3px 8px; font-size: 11px; }"
                "QPushButton:hover { background-color: #FFC0CB; color: #2C3E50; }"
                );
            bookRow->addWidget(btnMove);

            QPushButton *btnRemoveFromShelf = new QPushButton("✕", nullptr);
            btnRemoveFromShelf->setCursor(Qt::PointingHandCursor);
            btnRemoveFromShelf->setFixedWidth(28);
            btnRemoveFromShelf->setStyleSheet(
                "QPushButton { background-color: #FF69B4; color: white; border: none; border-radius: 6px; }"
                "QPushButton:hover { background-color: #FFC0CB; color: #2C3E50; }"
                );
            bookRow->addWidget(btnRemoveFromShelf);

            int bookId = book.getId();

            connect(btnMove, &QPushButton::clicked, this, [this, shelfName, bookId, moveCombo, book]() {
                if (!currentUser) return;
                QString targetShelfName = moveCombo->currentText();
                if (targetShelfName.isEmpty()) return;

                Shelf *sourceShelf = currentUser->findShelf(shelfName);
                Shelf *targetShelf = currentUser->findShelf(targetShelfName);
                if (sourceShelf && targetShelf) {
                    sourceShelf->removeBook(bookId);
                    targetShelf->addBook(book);
                    emit userUpdated(*currentUser);
                    refreshShelves();
                }
            });

            connect(btnRemoveFromShelf, &QPushButton::clicked, this, [this, shelfName, bookId]() {
                if (!currentUser) return;
                Shelf *targetShelf = currentUser->findShelf(shelfName);
                if (targetShelf) {
                    targetShelf->removeBook(bookId);
                    emit userUpdated(*currentUser);
                    refreshShelves();
                }
            });

            shelfBoxLayout->addLayout(bookRow);
        }

        shelvesLayout->addWidget(shelfBox);
    }
}

void PersonalLibraryWidget::onCreateShelfClicked()
{
    QString name = leNewShelfName->text().trimmed();
    if (name.isEmpty()) {
        QMessageBox::warning(this, "Error", "Please enter a shelf name.");
        return;
    }

    if (!currentUser) return;

    for (const Shelf &s : currentUser->getShelves()) {
        if (s.getName() == name) {
            QMessageBox::warning(this, "Error", "A shelf with this name already exists.");
            return;
        }
    }

    Shelf newShelf(name);
    currentUser->addShelf(newShelf);
    leNewShelfName->clear();

    emit userUpdated(*currentUser);
    refreshShelves();
}

void PersonalLibraryWidget::onBackClicked()
{
    emit backToHomeRequested();
}

void PersonalLibraryWidget::paintEvent(QPaintEvent *event)
{
    QStyleOption opt;
    opt.initFrom(this);
    QPainter painter(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &painter, this);
}