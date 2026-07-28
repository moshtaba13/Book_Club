#include "bookdetailwindow.h"
#include <QHBoxLayout>
#include <QMessageBox>
#include <QFrame>
#include <QStyleOption>
#include <QPainter>
#include <QPixmap>
#include "networkclient.h"
#include "modelserializer.h"
#include "bookcoverloader.h"

namespace {

QPixmap fitCoverToFrame(const QPixmap &source, const QSize &targetSize)
{
    if (source.isNull() || targetSize.isEmpty())
        return source;

    QPixmap scaled = source.scaled(targetSize, Qt::KeepAspectRatioByExpanding, Qt::SmoothTransformation);
    int x = (scaled.width() - targetSize.width()) / 2;
    int y = (scaled.height() - targetSize.height()) / 2;
    return scaled.copy(x, y, targetSize.width(), targetSize.height());
}
}

BookDetailWidget::BookDetailWidget(Cart *cart, User *user, QWidget *parent)
    : QWidget(parent), mainCart(cart), currentUserPtr(user)
{
    this->setObjectName("bookDetailPage");
    this->setStyleSheet("QWidget#bookDetailPage { background-color: #FCFCFC; }");

    QVBoxLayout *outerLayout = new QVBoxLayout(this);
    outerLayout->setContentsMargins(0, 0, 0, 0);
    outerLayout->setSpacing(0);


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
        "   color: #803040;"
        "   border: none;"
        "   border-radius: 15px;"
        "   padding-left: 15px;"
        "   padding-right: 15px;"
        "   min-height: 35px;"
        "   font-weight: bold;"
        "   font-size: 12px;"
        "}"
        "QPushButton:hover { background-color: rgba(244, 219, 222, 220); }"
        );
    headerLayout->addWidget(backButton);
    headerLayout->addStretch();
    outerLayout->addWidget(headerWidget);


    QScrollArea *scrollArea = new QScrollArea(this);
    scrollArea->setWidgetResizable(true);
    scrollArea->setFrameShape(QFrame::NoFrame);
    scrollArea->setStyleSheet(
        "QScrollArea { background: transparent; }"
        "QScrollArea > QWidget > QWidget { background: transparent; }"
        );

    QWidget *scrollWidget = new QWidget(scrollArea);
    scrollWidget->setStyleSheet("background-color: transparent;");
    QVBoxLayout *mainLayout = new QVBoxLayout(scrollWidget);
    mainLayout->setContentsMargins(30, 30, 30, 30);
    mainLayout->setSpacing(20);


    QFrame *infoCard = new QFrame(scrollWidget);
    infoCard->setStyleSheet("background-color: rgba(253, 246, 238, 180); border-radius: 20px;");
    QHBoxLayout *infoLayout = new QHBoxLayout(infoCard);
    infoLayout->setContentsMargins(25, 25, 25, 25);
    infoLayout->setSpacing(20);


    coverLabel = new QLabel(infoCard);
    coverLabel->setFixedSize(140, 190);
    coverLabel->setStyleSheet(
        "background-color: rgba(255, 255, 255, 180);"
        "border-radius: 12px;"
        "border: 1px solid rgba(0, 0, 0, 20);"
        );
    coverLabel->setAlignment(Qt::AlignCenter);
    infoLayout->addWidget(coverLabel);


    QVBoxLayout *textColumn = new QVBoxLayout();
    textColumn->setSpacing(8);

    titleLabel = new QLabel(infoCard);
    titleLabel->setFont(QFont("Segoe UI", 20, QFont::Bold));
    titleLabel->setStyleSheet("color: #706357; background: transparent;");
    titleLabel->setWordWrap(true);
    textColumn->addWidget(titleLabel);

    authorLabel = new QLabel(infoCard);
    authorLabel->setFont(QFont("Segoe UI", 13));
    authorLabel->setStyleSheet("color: #706357; background: transparent;");
    textColumn->addWidget(authorLabel);

    genreLabel = new QLabel(infoCard);
    genreLabel->setFont(QFont("Segoe UI", 11, QFont::DemiBold));
    genreLabel->setStyleSheet("color: #2E4D63; background: transparent;");
    textColumn->addWidget(genreLabel);

    ratingLabel = new QLabel(infoCard);
    ratingLabel->setFont(QFont("Segoe UI", 11));
    ratingLabel->setStyleSheet("color: #803040; background: transparent;");
    textColumn->addWidget(ratingLabel);

    descriptionLabel = new QLabel(infoCard);
    descriptionLabel->setFont(QFont("Segoe UI", 11));
    descriptionLabel->setStyleSheet("color: #706357; background: transparent;");
    descriptionLabel->setWordWrap(true);
    textColumn->addWidget(descriptionLabel);

    QHBoxLayout *priceRow = new QHBoxLayout();
    originalPriceLabel = new QLabel(infoCard);
    originalPriceLabel->setFont(QFont("Segoe UI", 11));
    originalPriceLabel->setStyleSheet("color: rgba(112, 99, 87, 120); text-decoration: line-through; background: transparent;");
    priceLabel = new QLabel(infoCard);
    priceLabel->setFont(QFont("Segoe UI", 18, QFont::Bold));
    priceLabel->setStyleSheet("color: #f68787; background: transparent;");
    priceRow->addWidget(originalPriceLabel);
    priceRow->addWidget(priceLabel);
    priceRow->addStretch();
    textColumn->addLayout(priceRow);

    addToCartButton = new QPushButton("Add to Cart 🛒", infoCard);
    addToCartButton->setCursor(Qt::PointingHandCursor);
    addToCartButton->setMinimumHeight(42);
    addToCartButton->setStyleSheet(
        "QPushButton {"
        "   background-color:  #FFD1DC;"
        "   color: #803040;"
        "   border: none;"
        "   border-radius: 8px;"
        "   font-weight: bold;"
        "}"
        "QPushButton:hover { background-color: rgba(244, 219, 222, 220); }"
        );
    textColumn->addWidget(addToCartButton);

    readButton = new QPushButton("📖 Read Book", infoCard);
    readButton->setCursor(Qt::PointingHandCursor);
    readButton->setMinimumHeight(42);
    readButton->setStyleSheet(
        "QPushButton {"
        "   background-color: #2C3E50;"
        "   color: white;"
        "   border: none;"
        "   border-radius: 8px;"
        "   font-weight: bold;"
        "}"
        "QPushButton:hover { background-color: #FFC0CB; color: #2C3E50; }"
        );
    textColumn->addWidget(readButton);

    infoLayout->addLayout(textColumn, 1);
    mainLayout->addWidget(infoCard);


    QFrame *reviewsCard = new QFrame(scrollWidget);
    reviewsCard->setStyleSheet("background-color: rgba(210, 235, 255, 170); border-radius: 20px;");
    QVBoxLayout *reviewsCardLayout = new QVBoxLayout(reviewsCard);
    reviewsCardLayout->setContentsMargins(25, 20, 25, 20);
    reviewsCardLayout->setSpacing(10);

    QLabel *reviewsTitle = new QLabel("Reviews", reviewsCard);
    reviewsTitle->setFont(QFont("Segoe UI", 15, QFont::Bold));
    reviewsTitle->setStyleSheet("color: #2E4D63; background: transparent;");
    reviewsCardLayout->addWidget(reviewsTitle);

    reviewsListLayout = new QVBoxLayout();
    reviewsListLayout->setSpacing(8);
    reviewsCardLayout->addLayout(reviewsListLayout);

    QFrame *divider = new QFrame(reviewsCard);
    divider->setFrameShape(QFrame::HLine);
    divider->setStyleSheet("background-color: rgba(46, 77, 99, 60); max-height: 1px; border: none;");
    reviewsCardLayout->addWidget(divider);

    QLabel *addReviewLabel = new QLabel("Write a review", reviewsCard);
    addReviewLabel->setFont(QFont("Segoe UI", 12, QFont::DemiBold));
    addReviewLabel->setStyleSheet("color: #2E4D63; background: transparent;");
    reviewsCardLayout->addWidget(addReviewLabel);

    starSelector = new QComboBox(reviewsCard);
    starSelector->addItems({"1 ★", "2 ★★", "3 ★★★", "4 ★★★★", "5 ★★★★★"});
    starSelector->setCurrentIndex(4);
    starSelector->setStyleSheet("background-color: rgba(255,255,255,210); border-radius: 6px; padding: 4px; color: #2E4D63;");
    reviewsCardLayout->addWidget(starSelector);

    reviewTextEdit = new QTextEdit(reviewsCard);
    reviewTextEdit->setPlaceholderText("Share your thoughts about this book...");
    reviewTextEdit->setMaximumHeight(70);
    reviewTextEdit->setStyleSheet("background-color: rgba(255,255,255,210); border-radius: 6px; padding: 6px; color: #2E4D63;");
    reviewsCardLayout->addWidget(reviewTextEdit);

    submitReviewButton = new QPushButton("Submit Review", reviewsCard);
    submitReviewButton->setCursor(Qt::PointingHandCursor);
    submitReviewButton->setStyleSheet(
        "QPushButton {"
        "   background-color: #FFD1DC;"
        "   color: #803040;"
        "   border: none;"
        "   border-radius: 8px;"
        "   font-weight: bold;"
        "   padding: 8px;"
        "}"
        "QPushButton:hover { background-color: rgba(244, 219, 222, 220); }"
        );
    reviewsCardLayout->addWidget(submitReviewButton);

    saveButton = new QPushButton("🔖 Save for Later", infoCard);
    saveButton->setCursor(Qt::PointingHandCursor);
    saveButton->setStyleSheet(
        "QPushButton { background-color: #FFC0CB; color: #2C3E50; border-radius: 8px; font-weight: bold; padding: 8px; }"
        "QPushButton:hover { background-color: #FFB6C1; }"
        );
    textColumn->addWidget(saveButton);

    connect(saveButton, &QPushButton::clicked, this, [this]() {
        QJsonObject data;
        data["book_id"] = currentBook.getId();

        QJsonObject response = NetworkClient::instance().sendRequest(RequestType::SaveBook, data);
        if (response.value("status").toString() != "Success") {
            QMessageBox::warning(this, "Error", response.value("message").toString("Could not save the book."));
            return;
        }
        QMessageBox::information(this, "Saved", "Book saved for later!");
    });

    mainLayout->addWidget(reviewsCard);
    mainLayout->addStretch();

    scrollArea->setWidget(scrollWidget);
    outerLayout->addWidget(scrollArea);

    connect(backButton, &QPushButton::clicked, this, &BookDetailWidget::onBackClicked);
    connect(addToCartButton, &QPushButton::clicked, this, &BookDetailWidget::onAddToCartClicked);
    connect(readButton, &QPushButton::clicked, this, &BookDetailWidget::onReadClicked);
    connect(submitReviewButton, &QPushButton::clicked, this, &BookDetailWidget::onSubmitReviewClicked);
}

QString BookDetailWidget::genreToString(genre g) const
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

void BookDetailWidget::loadBook(const Book &book)
{
    currentBook = book;
    resetReviewForm();


    QJsonObject response = NetworkClient::instance().sendRequest(
        RequestType::GetBookDetails, QJsonObject{{"book_id", book.getId()}});
    if (response.value("status").toString() == "Success")
        currentBook = ModelSerializer::bookFromJson(response.value("data").toObject());

    titleLabel->setText(currentBook.getTitle());
    authorLabel->setText("by " + currentBook.getAuthor());
    genreLabel->setText(genreToString(currentBook.getGenre()));
    descriptionLabel->setText(currentBook.getDiscription().isEmpty() ? "No description available." : currentBook.getDiscription());
    ratingLabel->setText(QString("⭐ %1 / 5").arg(currentBook.getAverageRating(), 0, 'f', 1));

    QPixmap pix = BookCoverLoader::instance().cover(currentBook.getId());
    if (!pix.isNull()) {
        coverLabel->setPixmap(fitCoverToFrame(pix, coverLabel->size()));
    } else {
        coverLabel->setPixmap(QPixmap());
        coverLabel->setText("📖");
        coverLabel->setFont(QFont("Segoe UI", 40));
    }

    if (currentBook.getPrice() > currentBook.getFinalPrice()) {
        originalPriceLabel->setText(QString("$%1").arg(currentBook.getPrice(), 0, 'f', 2));
        originalPriceLabel->show();
    } else {
        originalPriceLabel->hide();
    }
    priceLabel->setText(QString("$%1").arg(currentBook.getFinalPrice(), 0, 'f', 2));

    refreshReviews();
}

void BookDetailWidget::refreshReviews()
{
    QLayoutItem *child;
    while ((child = reviewsListLayout->takeAt(0)) != nullptr) {
        if (child->widget()) child->widget()->deleteLater();
        delete child;
    }

    if (currentBook.getReviews().isEmpty()) {
        QLabel *lblNone = new QLabel("No reviews yet. Be the first!");
        lblNone->setStyleSheet("color: #2E4D63; background: transparent;");
        reviewsListLayout->addWidget(lblNone);
        return;
    }

    for (const Review &r : currentBook.getReviews()) {
        QFrame *reviewRow = new QFrame();
        reviewRow->setStyleSheet("background-color: rgba(255,255,255,150); border-radius: 8px;");
        QHBoxLayout *reviewRowLayout = new QHBoxLayout(reviewRow);
        reviewRowLayout->setContentsMargins(10, 8, 10, 8);

        QLabel *lblReview = new QLabel(QString("%1 ★ — %2").arg(r.getStars()).arg(r.getComment()));
        lblReview->setWordWrap(true);
        lblReview->setStyleSheet("color: #2E4D63; background: transparent;");
        reviewRowLayout->addWidget(lblReview, 1);

        if (currentUserPtr && r.getUserId() == currentUserPtr->getId()) {
            int reviewUserId = r.getUserId();
            int reviewStars = r.getStars();
            QString reviewComment = r.getComment();

            QPushButton *btnEdit = new QPushButton("✏️ Edit", reviewRow);
            btnEdit->setCursor(Qt::PointingHandCursor);
            btnEdit->setStyleSheet(
                "QPushButton { background-color: #FFD1DC; color: #803040; border: none; border-radius: 6px; padding: 4px 10px; }"
                "QPushButton:hover { background-color: rgba(244, 219, 222, 220); }"
                );
            reviewRowLayout->addWidget(btnEdit);

            QPushButton *btnDelete = new QPushButton("🗑️ Delete", reviewRow);
            btnDelete->setCursor(Qt::PointingHandCursor);
            btnDelete->setStyleSheet(
                "QPushButton { background-color: #FF69B4; color: white; border: none; border-radius: 6px; padding: 4px 10px; }"
                "QPushButton:hover { background-color: #FFC0CB; color: #2C3E50; }"
                );
            reviewRowLayout->addWidget(btnDelete);

            connect(btnEdit, &QPushButton::clicked, this, [this, reviewStars, reviewComment]() {
                editingReview = true;
                starSelector->setCurrentIndex(reviewStars - 1);
                reviewTextEdit->setPlainText(reviewComment);
                submitReviewButton->setText("Update Review");
            });

            connect(btnDelete, &QPushButton::clicked, this, [this, reviewUserId]() {
                Q_UNUSED(reviewUserId);
                if (QMessageBox::question(this, "Delete Review", "Are you sure you want to delete your review?")
                    != QMessageBox::Yes)
                    return;

                QJsonObject data;
                data["book_id"] = currentBook.getId();
                QJsonObject response = NetworkClient::instance().sendRequest(RequestType::DeleteReview, data);
                if (response.value("status").toString() != "Success") {
                    QMessageBox::warning(this, "Error", response.value("message").toString("Could not delete the review."));
                    return;
                }

                loadBook(currentBook);
            });
        }

        reviewsListLayout->addWidget(reviewRow);
    }
}

void BookDetailWidget::resetReviewForm()
{
    editingReview = false;
    reviewTextEdit->clear();
    starSelector->setCurrentIndex(4);
    submitReviewButton->setText("Submit Review");
}

void BookDetailWidget::onAddToCartClicked()
{
    QJsonObject data;
    data["book_id"] = currentBook.getId();

    QJsonObject response = NetworkClient::instance().sendRequest(RequestType::AddToCart, data);
    if (response.value("status").toString() != "Success") {
        QMessageBox::warning(this, "Could Not Add to Cart",
                             response.value("message").toString("This book could not be added to your cart."));
        return;
    }

    QMessageBox::information(this, "Added", "Book added to your cart.");
}

void BookDetailWidget::onReadClicked()
{
    emit readRequested(currentBook.getId(), currentBook.getTitle());
}

void BookDetailWidget::onSubmitReviewClicked()
{
    if (reviewTextEdit->toPlainText().trimmed().isEmpty()) {
        QMessageBox::warning(this, "Empty review", "Please write something before submitting.");
        return;
    }

    if (!currentUserPtr) return;

    QJsonObject data;
    data["book_id"] = currentBook.getId();
    data["stars"] = starSelector->currentIndex() + 1;
    data["comment"] = reviewTextEdit->toPlainText().trimmed();

    QJsonObject response = NetworkClient::instance().sendRequest(
        editingReview ? RequestType::EditReview : RequestType::AddReview, data);

    if (response.value("status").toString() != "Success") {
        QMessageBox::warning(this, "Error", response.value("message").toString("Could not submit the review."));
        return;
    }

    loadBook(currentBook);
}

void BookDetailWidget::onBackClicked()
{
    emit backToHomeRequested();
}

void BookDetailWidget::paintEvent(QPaintEvent *event)
{
    QStyleOption opt;
    opt.initFrom(this);
    QPainter painter(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &painter, this);
}