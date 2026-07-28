#include "seeallbookswidget.h"
#include <QStyleOption>
#include <QPainter>
#include <QFrame>
#include <QHBoxLayout>
#include "bookcardwidget.h"
#include "protocol.h"

namespace {
constexpr int kColumns = 4;
}

SeeAllBooksWidget::SeeAllBooksWidget(QWidget *parent) : QWidget(parent)
{
    this->setObjectName("seeAllPage");
    this->setStyleSheet(
        "QWidget#seeAllPage {"
        "   background-color: #FCFCFC;"
        "}"
        );

    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setSpacing(0);


    headerWidget = new QWidget(this);
    headerWidget->setMinimumHeight(75);
    headerWidget->setMaximumHeight(75);
    headerWidget->setStyleSheet("background-color: rgba(210, 235, 255, 170); border-bottom: 1px solid rgba(0,0,0,15);");

    QHBoxLayout *headerLayout = new QHBoxLayout(headerWidget);
    headerLayout->setContentsMargins(25, 0, 25, 0);
    headerLayout->setSpacing(15);

    btnBack = new QPushButton("⬅ Back", headerWidget);
    btnBack->setCursor(Qt::PointingHandCursor);
    btnBack->setStyleSheet(
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
    headerLayout->addWidget(btnBack);

    headerLayout->addStretch(1);

    lblTitle = new QLabel(headerWidget);
    lblTitle->setFont(QFont("Segoe UI", 18, QFont::Bold));
    lblTitle->setStyleSheet("color: #2C3E50; background: transparent;");
    headerLayout->addWidget(lblTitle);

    headerLayout->addStretch(1);


    QWidget *spacer = new QWidget(headerWidget);
    spacer->setFixedWidth(70);
    headerLayout->addWidget(spacer);

    mainLayout->addWidget(headerWidget);


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

    QFrame *booksFrame = new QFrame(scrollWidget);
    booksFrame->setStyleSheet("background-color: rgba(253, 246, 238, 180); border-radius: 20px;");
    QVBoxLayout *booksFrameLayout = new QVBoxLayout(booksFrame);
    booksFrameLayout->setContentsMargins(20, 18, 20, 18);

    gridLayout = new QGridLayout();
    gridLayout->setSpacing(15);
    booksFrameLayout->addLayout(gridLayout);

    scrollLayout->addWidget(booksFrame);
    scrollLayout->addStretch(1);

    scrollArea->setWidget(scrollWidget);
    mainLayout->addWidget(scrollArea);

    connect(btnBack, &QPushButton::clicked, this, &SeeAllBooksWidget::backRequested);
}

static void clearGrid(QGridLayout *layout)
{
    QLayoutItem *child;
    while ((child = layout->takeAt(0)) != nullptr) {
        if (child->widget()) {
            child->widget()->deleteLater();
        }
        delete child;
    }
}

void SeeAllBooksWidget::loadBooks(const QString &sectionTitle, const QVector<Book> &books)
{
    lblTitle->setText(sectionTitle);

    clearGrid(gridLayout);

    if (books.isEmpty()) {
        QLabel *lblNone = new QLabel("No books here yet.");
        lblNone->setStyleSheet("color: #706357; background: transparent;");
        gridLayout->addWidget(lblNone, 0, 0);
        return;
    }

    for (int i = 0; i < books.size(); ++i) {
        const Book &book = books.at(i);
        QPushButton *card = BookCardWidget::create(book, scrollWidget);
        connect(card, &QPushButton::clicked, this, [this, book]() {
            emit bookClicked(book);
        });
        gridLayout->addWidget(card, i / kColumns, i % kColumns);
    }
}

void SeeAllBooksWidget::loadGenreHighlights(const QString &sectionTitle, const QVector<Book> &highlightPerGenre)
{
    lblTitle->setText(sectionTitle);

    clearGrid(gridLayout);

    if (highlightPerGenre.isEmpty()) {
        QLabel *lblNone = new QLabel("No genres here yet.");
        lblNone->setStyleSheet("color: #706357; background: transparent;");
        gridLayout->addWidget(lblNone, 0, 0);
        return;
    }

    for (int i = 0; i < highlightPerGenre.size(); ++i) {
        const Book &highlightBook = highlightPerGenre.at(i);
        genre g = highlightBook.getGenre();
        QPushButton *card = BookCardWidget::create(highlightBook, scrollWidget, genreDisplayName(g));
        connect(card, &QPushButton::clicked, this, [this, g]() {
            emit genreClicked(g);
        });
        gridLayout->addWidget(card, i / kColumns, i % kColumns);
    }
}

void SeeAllBooksWidget::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    QStyleOption opt;
    opt.initFrom(this);

    QPainter painter(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &painter, this);
}

SeeAllBooksWidget::~SeeAllBooksWidget() = default;
