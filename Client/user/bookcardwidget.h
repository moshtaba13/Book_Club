#ifndef BOOKCARDWIDGET_H
#define BOOKCARDWIDGET_H

#include <QPushButton>
#include <QString>
#include "Book.h"


namespace BookCardWidget
{

    QPushButton *create(const Book &book, QWidget *parent, const QString &captionOverride = QString());
}

#endif // BOOKCARDWIDGET_H
