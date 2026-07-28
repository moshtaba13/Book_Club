#ifndef BOOKCOVERLOADER_H
#define BOOKCOVERLOADER_H

#include <QPixmap>
#include <QHash>


class BookCoverLoader
{
public:
    static BookCoverLoader& instance();

    QPixmap cover(int bookId);

    void clearCache();

private:
    BookCoverLoader() = default;
    BookCoverLoader(const BookCoverLoader&) = delete;
    BookCoverLoader& operator=(const BookCoverLoader&) = delete;

    QHash<int, QPixmap> cache;
};

#endif // BOOKCOVERLOADER_H
