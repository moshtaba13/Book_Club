#ifndef BOOKCOVERLOADER_H
#define BOOKCOVERLOADER_H

#include <QPixmap>
#include <QHash>

// The book cover image file lives on the server's disk - Book::getImagePath()
// is a server-side path and is meaningless on the client machine. Covers must
// be fetched over the network via the GetBookCover request (already
// implemented server-side) and decoded from the base64 bytes it returns.
//
// This loader fetches a cover once per book id and keeps it in memory for the
// rest of the session, so re-opening the same book or scrolling the same list
// doesn't re-download the image every time.
class BookCoverLoader
{
public:
    static BookCoverLoader& instance();

    // Returns the cover for bookId, fetching it from the server on first
    // request and caching the result. Returns a null QPixmap if the book has
    // no cover or the fetch failed - callers should fall back to a
    // placeholder in that case.
    QPixmap cover(int bookId);

    void clearCache();

private:
    BookCoverLoader() = default;
    BookCoverLoader(const BookCoverLoader&) = delete;
    BookCoverLoader& operator=(const BookCoverLoader&) = delete;

    QHash<int, QPixmap> cache;
};

#endif // BOOKCOVERLOADER_H
