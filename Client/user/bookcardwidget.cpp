#include "bookcardwidget.h"
#include "bookcoverloader.h"
#include <QSizePolicy>
#include <QIcon>
#include <QPixmap>

namespace {
constexpr int kCardWidth = 130;
constexpr int kCardHeight = 185;
constexpr int kCoverWidth = 96;
constexpr int kCoverHeight = 128;
}

QPushButton *BookCardWidget::create(const Book &book, QWidget *parent, const QString &captionOverride)
{
    QPushButton *card = new QPushButton(parent);
    card->setCursor(Qt::PointingHandCursor);

    // Fixed frame: identical size for every card no matter the cover's
    // own dimensions, so the grid stays perfectly aligned.
    card->setFixedSize(kCardWidth, kCardHeight);
    card->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

    card->setStyleSheet(
        "QPushButton {"
        "   background-color: rgba(255, 255, 255, 110);"
        "   border-radius: 12px;"
        "   border: 1px solid rgba(0, 0, 0, 20);"
        "   padding: 5px;"
        "   text-align: bottom;"
        "}"
        "QPushButton:hover {"
        "   background-color: rgba(255, 255, 255, 180);"
        "}"
        );

    QString caption = captionOverride.isEmpty() ? book.getTitle() : captionOverride;

    QPixmap cover = BookCoverLoader::instance().cover(book.getId());
    QSize iconSize(kCoverWidth, kCoverHeight);
    if (!cover.isNull()) {
        // Show the real cover as the button icon, always scaled into the
        // same fixed icon box, with the caption as a small line underneath.
        card->setIcon(QIcon(cover.scaled(iconSize, Qt::KeepAspectRatio, Qt::SmoothTransformation)));
        card->setIconSize(iconSize);
        card->setText("\n" + caption);
    } else {
        // No cover available - fall back to just the caption text, still
        // inside the same fixed frame.
        card->setText(caption);
    }

    return card;
}
