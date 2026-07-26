#include "home.h"
#include <QStyleOption>
#include <QPainter>
#include <QPixmap>

home::home(QWidget *parent) : QWidget(parent) {
    this->setObjectName("homePage");
    this->setStyleSheet(
        "QWidget#homePage {"
        "   background-color: #FCFCFC;"
        "}"
        );

    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setSpacing(0);

    // نوار هدر بالا
    headerWidget = new QWidget(this);
    headerWidget->setMinimumHeight(75);
    headerWidget->setMaximumHeight(75);
    headerWidget->setStyleSheet("background-color: rgba(210, 235, 255, 170);  border-bottom: 1px solid rgba(0,0,0,15);");

    QHBoxLayout *headerLayout = new QHBoxLayout(headerWidget);
    headerLayout->setContentsMargins(25, 0, 25, 0);
    headerLayout->setSpacing(15);

    lblLogo = new QLabel(headerWidget);
    lblLogo->setText("<span style='color:#f68787;'>B👓k Club</span>");
    lblLogo->setFont(QFont("Segoe UI", 22, QFont::Bold));
    headerLayout->addWidget(lblLogo);

    headerLayout->addStretch(1);

    leSearch = new QLineEdit(headerWidget);
    leSearch->setPlaceholderText("search...");
    leSearch->setFixedWidth(240);
    leSearch->setMinimumHeight(36);
    leSearch->setStyleSheet(
        "QLineEdit {"
        "   border: 1px solid #000000;"
        "   border-radius: 18px;"
        "   padding-left: 15px;"
        "   padding-right: 15px;"
        "   color: #f68787;"
        "   background-color: rgba(255, 255, 255, 180);"
        "}"
        );
    headerLayout->addWidget(leSearch);

    headerLayout->addStretch(1);

    btnNotification = new QPushButton("🔔", headerWidget);
    btnNotification->setCursor(Qt::PointingHandCursor);
    btnNotification->setStyleSheet("background: transparent; border: none; font-size: 19px; color: #000000;");
    headerLayout->addWidget(btnNotification);

    btnCart = new QPushButton("🛒", headerWidget);
    btnCart->setCursor(Qt::PointingHandCursor);
    btnCart->setStyleSheet("background: transparent; border: none; font-size: 19px; color: #000000;");
    headerLayout->addWidget(btnCart);

    btnProfile = new QPushButton("👤", headerWidget);
    btnProfile->setCursor(Qt::PointingHandCursor);
    btnProfile->setStyleSheet("background: transparent; border: none; font-size: 21px; color: #FF69B4;");
    btnLibrary = new QPushButton("📚", headerWidget);
    btnLibrary->setCursor(Qt::PointingHandCursor);
    btnLibrary->setStyleSheet("background: transparent; border: none; font-size: 19px; color: #000000;");
    headerLayout->addWidget(btnLibrary);
    headerLayout->addWidget(btnProfile);

    mainLayout->addWidget(headerWidget);

    // منطقه اسکرول‌شونده
    scrollArea = new QScrollArea(this);
    scrollArea->setWidgetResizable(true);
    scrollArea->setFrameShape(QFrame::NoFrame);
    scrollArea->setStyleSheet(
        "QScrollArea { background: transparent; }"
        "QScrollArea > QWidget > QWidget { background: transparent; }"
        );

    scrollWidget = new QWidget(scrollArea);
    scrollWidget->setStyleSheet("background-color: transparent;");
    QVBoxLayout *scrollLayout = new QVBoxLayout(scrollWidget);
    scrollLayout->setContentsMargins(20, 20, 20, 20);
    scrollLayout->setSpacing(25);

    // الف) بخش ژانرها (Genres)
    genresFrame = new QFrame(scrollWidget);
    genresFrame->setStyleSheet("background-color: rgba(253, 246, 238, 180); border-radius: 20px;");
    QVBoxLayout *genresLayout = new QVBoxLayout(genresFrame);
    genresLayout->setContentsMargins(20, 18, 20, 18);

    QHBoxLayout *genresHeader = new QHBoxLayout();
    QLabel *lblGenresTitle = new QLabel("Genres", genresFrame);
    lblGenresTitle->setFont(QFont("Segoe UI", 14, QFont::Bold));
    lblGenresTitle->setStyleSheet("color: #706357; background: transparent;");
    QPushButton *btnGenresSeeAll = new QPushButton("See all", genresFrame);
    btnGenresSeeAll->setStyleSheet("color: #000000; background: transparent; border: none; font-weight: bold; font-size: 12px;");
    btnGenresSeeAll->setCursor(Qt::PointingHandCursor);
    genresHeader->addWidget(lblGenresTitle);
    genresHeader->addStretch();
    genresHeader->addWidget(btnGenresSeeAll);
    genresLayout->addLayout(genresHeader);

    genresItemsLayout = new QHBoxLayout();
    genresItemsLayout->setSpacing(15);
    genresLayout->addLayout(genresItemsLayout);

    scrollLayout->addWidget(genresFrame);

    // ب) بخش ویژه (Featured Books)
    featuredFrame = new QFrame(scrollWidget);
    featuredFrame->setStyleSheet("background-color: rgba(210, 235, 255, 170); border-radius: 20px;");
    QVBoxLayout *featuredLayout = new QVBoxLayout(featuredFrame);
    featuredLayout->setContentsMargins(20, 18, 20, 18);

    QHBoxLayout *featuredHeader = new QHBoxLayout();
    QLabel *lblFeaturedTitle = new QLabel("Featured Books", featuredFrame);
    lblFeaturedTitle->setFont(QFont("Segoe UI", 14, QFont::Bold));
    lblFeaturedTitle->setStyleSheet("color: #2E4D63; background: transparent;");
    QPushButton *btnFeaturedSeeAll = new QPushButton("See all", featuredFrame);
    btnFeaturedSeeAll->setStyleSheet("color: #000000; background: transparent; border: none; font-weight: bold; font-size: 12px;");
    btnFeaturedSeeAll->setCursor(Qt::PointingHandCursor);
    featuredHeader->addWidget(lblFeaturedTitle);
    featuredHeader->addStretch();
    featuredHeader->addWidget(btnFeaturedSeeAll);
    featuredLayout->addLayout(featuredHeader);

    featuredItemsLayout = new QHBoxLayout();
    featuredItemsLayout->setSpacing(15);
    featuredLayout->addLayout(featuredItemsLayout);

    scrollLayout->addWidget(featuredFrame);

    // ج) بخش پیشنهادی (Recommended for u)
    recommendedFrame = new QFrame(scrollWidget);
    recommendedFrame->setStyleSheet("background-color: #FFD1DC; border-radius: 20px;");
    QVBoxLayout *recommendedLayout = new QVBoxLayout(recommendedFrame);
    recommendedLayout->setContentsMargins(20, 18, 20, 18);

    QHBoxLayout *recommendedHeader = new QHBoxLayout();
    QLabel *lblRecommendedTitle = new QLabel("Recommended for u", recommendedFrame);
    lblRecommendedTitle->setFont(QFont("Segoe UI", 14, QFont::Bold));
    lblRecommendedTitle->setStyleSheet("color: #803040; background: transparent;");
    QPushButton *btnRecommendedSeeAll = new QPushButton("See all", recommendedFrame);
    btnRecommendedSeeAll->setStyleSheet("color: #000000; background: transparent; border: none; font-weight: bold; font-size: 12px;");
    btnRecommendedSeeAll->setCursor(Qt::PointingHandCursor);
    recommendedHeader->addWidget(lblRecommendedTitle);
    recommendedHeader->addStretch();
    recommendedHeader->addWidget(btnRecommendedSeeAll);
    recommendedLayout->addLayout(recommendedHeader);

    recommendedItemsLayout = new QHBoxLayout();
    recommendedItemsLayout->setSpacing(15);
    recommendedLayout->addLayout(recommendedItemsLayout);

    scrollLayout->addWidget(recommendedFrame);

    // د) بخش جدیدترین‌ها (New Releases)
    newReleasesFrame = new QFrame(scrollWidget);
    newReleasesFrame->setStyleSheet("background-color: rgba(255, 192, 159, 195); border-radius: 20px;");
    QVBoxLayout *newReleasesLayout = new QVBoxLayout(newReleasesFrame);
    newReleasesLayout->setContentsMargins(20, 18, 20, 18);

    QHBoxLayout *newReleasesHeader = new QHBoxLayout();
    QLabel *lblNewReleasesTitle = new QLabel("New Releases", newReleasesFrame);
    lblNewReleasesTitle->setFont(QFont("Segoe UI", 14, QFont::Bold));
    lblNewReleasesTitle->setStyleSheet("color: #A04E20; background: transparent;");
    QPushButton *btnNewReleasesSeeAll = new QPushButton("See all", newReleasesFrame);
    btnNewReleasesSeeAll->setStyleSheet("color: #000000; background: transparent; border: none; font-weight: bold; font-size: 12px;");
    btnNewReleasesSeeAll->setCursor(Qt::PointingHandCursor);
    newReleasesHeader->addWidget(lblNewReleasesTitle);
    newReleasesHeader->addStretch();
    newReleasesHeader->addWidget(btnNewReleasesSeeAll);
    newReleasesLayout->addLayout(newReleasesHeader);

    newReleasesItemsLayout = new QHBoxLayout();
    newReleasesItemsLayout->setSpacing(15);
    newReleasesLayout->addLayout(newReleasesItemsLayout);

    scrollLayout->addWidget(newReleasesFrame);

    // هـ) بخش پرفروش‌ها (Best Sellers)
    bestSellersFrame = new QFrame(scrollWidget);
    bestSellersFrame->setStyleSheet("background-color: rgba(210, 235, 255, 170); border-radius: 20px;");
    QVBoxLayout *bestSellersLayout = new QVBoxLayout(bestSellersFrame);
    bestSellersLayout->setContentsMargins(20, 18, 20, 18);

    QHBoxLayout *bestSellersHeader = new QHBoxLayout();
    QLabel *lblBestSellersTitle = new QLabel("Best Sellers", bestSellersFrame);
    lblBestSellersTitle->setFont(QFont("Segoe UI", 14, QFont::Bold));
    lblBestSellersTitle->setStyleSheet("color: #2E4D63; background: transparent;");
    QPushButton *btnBestSellersSeeAll = new QPushButton("See all", bestSellersFrame);
    btnBestSellersSeeAll->setStyleSheet("color: #000000; background: transparent; border: none; font-weight: bold; font-size: 12px;");
    btnBestSellersSeeAll->setCursor(Qt::PointingHandCursor);
    bestSellersHeader->addWidget(lblBestSellersTitle);
    bestSellersHeader->addStretch();
    bestSellersHeader->addWidget(btnBestSellersSeeAll);
    bestSellersLayout->addLayout(bestSellersHeader);

    bestSellersItemsLayout = new QHBoxLayout();
    bestSellersItemsLayout->setSpacing(15);
    bestSellersLayout->addLayout(bestSellersItemsLayout);

    scrollLayout->addWidget(bestSellersFrame);

    // و) بخش کتاب‌های رایگان (Free Books)
    freeBooksFrame = new QFrame(scrollWidget);
    freeBooksFrame->setStyleSheet("background-color: #FFD1DC; border-radius: 20px;");
    QVBoxLayout *freeBooksLayout = new QVBoxLayout(freeBooksFrame);
    freeBooksLayout->setContentsMargins(20, 18, 20, 18);

    QHBoxLayout *freeBooksHeader = new QHBoxLayout();
    QLabel *lblFreeBooksTitle = new QLabel("Free Books", freeBooksFrame);
    lblFreeBooksTitle->setFont(QFont("Segoe UI", 14, QFont::Bold));
    lblFreeBooksTitle->setStyleSheet("color: #803040; background: transparent;");
    QPushButton *btnFreeBooksSeeAll = new QPushButton("See all", freeBooksFrame);
    btnFreeBooksSeeAll->setStyleSheet("color: #000000; background: transparent; border: none; font-weight: bold; font-size: 12px;");
    btnFreeBooksSeeAll->setCursor(Qt::PointingHandCursor);
    freeBooksHeader->addWidget(lblFreeBooksTitle);
    freeBooksHeader->addStretch();
    freeBooksHeader->addWidget(btnFreeBooksSeeAll);
    freeBooksLayout->addLayout(freeBooksHeader);

    freeBooksItemsLayout = new QHBoxLayout();
    freeBooksItemsLayout->setSpacing(15);
    freeBooksLayout->addLayout(freeBooksItemsLayout);

    scrollLayout->addWidget(freeBooksFrame);

    // ز) نتایج جستجو (مخفی، فقط وقتی جستجو انجام بشه نشون داده می‌شه)
    searchResultsFrame = new QFrame(scrollWidget);
    searchResultsFrame->setStyleSheet("background-color: rgba(253, 246, 238, 180); border-radius: 20px;");
    QVBoxLayout *searchResultsLayout = new QVBoxLayout(searchResultsFrame);
    searchResultsLayout->setContentsMargins(20, 18, 20, 18);

    QHBoxLayout *searchResultsHeader = new QHBoxLayout();
    lblSearchResultsTitle = new QLabel("Search Results", searchResultsFrame);
    lblSearchResultsTitle->setFont(QFont("Segoe UI", 14, QFont::Bold));
    lblSearchResultsTitle->setStyleSheet("color: #706357; background: transparent;");
    QPushButton *btnClearSearch = new QPushButton("✕ Clear search", searchResultsFrame);
    btnClearSearch->setCursor(Qt::PointingHandCursor);
    btnClearSearch->setStyleSheet("color: #803040; background: transparent; border: none; font-weight: bold; font-size: 12px;");
    searchResultsHeader->addWidget(lblSearchResultsTitle);
    searchResultsHeader->addStretch();
    searchResultsHeader->addWidget(btnClearSearch);
    searchResultsLayout->addLayout(searchResultsHeader);

    connect(btnClearSearch, &QPushButton::clicked, this, [this]() {
        leSearch->clear();
        clearSearchResults();
    });

    searchResultsItemsLayout = new QHBoxLayout();
    searchResultsItemsLayout->setSpacing(15);
    searchResultsLayout->addLayout(searchResultsItemsLayout);

    scrollLayout->addWidget(searchResultsFrame);
    searchResultsFrame->hide();

    scrollArea->setWidget(scrollWidget);
    mainLayout->addWidget(scrollArea);

    connect(btnCart, &QPushButton::clicked, this, &home::cartRequested);

    connect(btnNotification, &QPushButton::clicked, this, &home::notificationsRequested);

    connect(leSearch, &QLineEdit::returnPressed, this, [this]() {
        QString query = leSearch->text().trimmed();
        if (query.isEmpty()) {
            clearSearchResults();
        } else {
            emit searchRequested(query);
        }
    });

    connect(btnProfile, &QPushButton::clicked, this, &home::profileRequested);
    connect(btnLibrary, &QPushButton::clicked, this, &home::libraryRequested);
}

QWidget *home::createBookWidget(const Book &book, QWidget *parent)
{
    QPushButton *card = new QPushButton(parent);
    card->setCursor(Qt::PointingHandCursor);

    card->setMinimumSize(110, 155);
    card->setMaximumSize(140, 180);
    card->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

    card->setText(book.getTitle());
    card->setStyleSheet(
        "QPushButton {"
        "   background-color: rgba(255, 255, 255, 110);"
        "   border-radius: 12px;"
        "   border: 1px solid rgba(0, 0, 0, 20);"
        "   padding: 5px;"
        "}"
        "QPushButton:hover {"
        "   background-color: rgba(255, 255, 255, 180);"
        "}"
        );

    connect(card, &QPushButton::clicked, this, [this, book]() {
        emit bookClicked(book);
    });

    return card;
}

static void clearLayout(QHBoxLayout *layout)
{
    QLayoutItem *child;
    while ((child = layout->takeAt(0)) != nullptr) {
        if (child->widget()) {
            child->widget()->deleteLater();
        }
        delete child;
    }
}

void home::loadGenreBooks(const QVector<Book> &books)
{
    clearLayout(genresItemsLayout);
    for (const Book &book : books) {
        genresItemsLayout->addWidget(createBookWidget(book, genresFrame));
    }
}

void home::loadFeaturedBooks(const QVector<Book> &books)
{
    clearLayout(featuredItemsLayout);
    for (const Book &book : books) {
        featuredItemsLayout->addWidget(createBookWidget(book, featuredFrame));
    }
}

void home::loadRecommendedBooks(const QVector<Book> &books)
{
    clearLayout(recommendedItemsLayout);
    for (const Book &book : books) {
        recommendedItemsLayout->addWidget(createBookWidget(book, recommendedFrame));
    }
}

void home::loadNewReleases(const QVector<Book> &books)
{
    clearLayout(newReleasesItemsLayout);
    for (const Book &book : books) {
        newReleasesItemsLayout->addWidget(createBookWidget(book, newReleasesFrame));
    }
}

void home::loadBestSellers(const QVector<Book> &books)
{
    clearLayout(bestSellersItemsLayout);
    for (const Book &book : books) {
        bestSellersItemsLayout->addWidget(createBookWidget(book, bestSellersFrame));
    }
}

void home::loadFreeBooks(const QVector<Book> &books)
{
    clearLayout(freeBooksItemsLayout);
    for (const Book &book : books) {
        freeBooksItemsLayout->addWidget(createBookWidget(book, freeBooksFrame));
    }
}

void home::paintEvent(QPaintEvent *event)
{
    QStyleOption opt;
    opt.initFrom(this);

    QPainter painter(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &painter, this);
}
void home::showSearchResults(const QVector<Book> &results, const QString &query)
{
    genresFrame->hide();
    featuredFrame->hide();
    recommendedFrame->hide();
    newReleasesFrame->hide();
    bestSellersFrame->hide();
    freeBooksFrame->hide();

    clearLayout(searchResultsItemsLayout);

    lblSearchResultsTitle->setText(QString("Search Results for \"%1\" (%2 found)").arg(query).arg(results.size()));

    if (results.isEmpty()) {
        QLabel *lblNone = new QLabel("No books found.");
        lblNone->setStyleSheet("color: #706357; background: transparent;");
        searchResultsItemsLayout->addWidget(lblNone);
    } else {
        for (const Book &book : results) {
            searchResultsItemsLayout->addWidget(createBookWidget(book, searchResultsFrame));
        }
    }

    searchResultsFrame->show();
}

void home::clearSearchResults()
{
    searchResultsFrame->hide();
    genresFrame->show();
    featuredFrame->show();
    recommendedFrame->show();
    newReleasesFrame->show();
    bestSellersFrame->show();
    freeBooksFrame->show();
}
home::~home() = default;