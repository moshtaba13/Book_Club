#ifndef BOOKCARDWIDGET_H
#define BOOKCARDWIDGET_H

#include <QPushButton>
#include <QString>
#include "Book.h"

// Shared factory for the small book "card" used both on the Home page and
// on the "See all" listing pages. Every card gets the exact same fixed
// frame size, so whichever cover ends up inside it (or if no cover has
// loaded yet) the layout never shifts around.
namespace BookCardWidget
{
    // captionOverride replaces the book title under the cover when set -
    // used for the Genres row, where the caption is the genre's name
    // rather than the title of whichever book represents it.
    QPushButton *create(const Book &book, QWidget *parent, const QString &captionOverride = QString());
}

#endif // BOOKCARDWIDGET_H
