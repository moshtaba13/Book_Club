#include "bookcardwidget.h"
#include "bookcoverloader.h"
#include <QSizePolicy>
#include <QPixmap>
#include <QPainter>
#include <QPainterPath>
#include <QVBoxLayout>
#include <QLabel>

namespace {
constexpr int kCardWidth = 130;
constexpr int kCardHeight = 185;
constexpr int kCoverWidth = 130;
constexpr int kCoverHeight = 150;
constexpr int kCoverCornerRadius = 12;

QPixmap fitCoverToFrame(const QPixmap &source, const QSize &targetSize)
{
    if (source.isNull() || targetSize.isEmpty())
        return source;

    QPixmap scaled = source.scaled(targetSize, Qt::KeepAspectRatioByExpanding, Qt::SmoothTransformation);
    int x = (scaled.width() - targetSize.width()) / 2;
    int y = (scaled.height() - targetSize.height()) / 2;
    return scaled.copy(x, y, targetSize.width(), targetSize.height());
}

QPixmap roundTopCorners(const QPixmap &source, int radius)
{
    if (source.isNull())
        return source;

    QPixmap rounded(source.size());
    rounded.fill(Qt::transparent);

    QPainter painter(&rounded);
    painter.setRenderHint(QPainter::Antialiasing);

    QRectF full(rounded.rect());
    QPainterPath path;
    path.addRoundedRect(full, radius, radius);

    QPainterPath squareBottom;
    squareBottom.addRect(0, full.height() / 2.0, full.width(), full.height() / 2.0);
    path = path.united(squareBottom);

    painter.setClipPath(path);
    painter.drawPixmap(0, 0, source);
    return rounded;
}
}

QPushButton *BookCardWidget::create(const Book &book, QWidget *parent, const QString &captionOverride)
{
    QPushButton *card = new QPushButton(parent);
    card->setCursor(Qt::PointingHandCursor);

    card->setFixedSize(kCardWidth, kCardHeight);
    card->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

    card->setStyleSheet(
        "QPushButton {"
        "   background-color: rgba(255, 255, 255, 210);"
        "   border-radius: 12px;"
        "   border: 1px solid rgba(0, 0, 0, 30);"
        "   padding: 0px;"
        "}"
        "QPushButton:hover {"
        "   background-color: rgba(255, 255, 255, 255);"
        "}"
        );


    QVBoxLayout *layout = new QVBoxLayout(card);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(0);


    QLabel *coverLabel = new QLabel(card);
    coverLabel->setFixedSize(kCoverWidth, kCoverHeight);
    coverLabel->setAttribute(Qt::WA_TransparentForMouseEvents); // Clicks pass through to the button
    coverLabel->setStyleSheet("background: transparent; border: none;");


    QLabel *titleLabel = new QLabel(card);
    titleLabel->setFixedHeight(kCardHeight - kCoverHeight); // 35px height
    titleLabel->setAlignment(Qt::AlignCenter);
    titleLabel->setWordWrap(true);
    titleLabel->setAttribute(Qt::WA_TransparentForMouseEvents); // Clicks pass through to the button
    titleLabel->setStyleSheet(
        "color: #2c3e50;"
        "font-weight: bold;"
        "font-size: 11px;"
        "background: transparent;"
        "border: none;"
        "padding: 2px 4px;"
        );

    QString caption = captionOverride.isEmpty() ? book.getTitle() : captionOverride;
    titleLabel->setText(caption);

    QPixmap cover = BookCoverLoader::instance().cover(book.getId());
    QSize iconSize(kCoverWidth, kCoverHeight);

    if (!cover.isNull()) {
        QPixmap filled = roundTopCorners(fitCoverToFrame(cover, iconSize), kCoverCornerRadius);
        coverLabel->setPixmap(filled);
    } else {
        coverLabel->setText("No Cover");
        coverLabel->setAlignment(Qt::AlignCenter);
        coverLabel->setStyleSheet("color: #888; font-size: 10px; background: transparent; border: none;");
    }

    layout->addWidget(coverLabel);
    layout->addWidget(titleLabel);

    return card;
}