#include "bookcoverloader.h"
#include "networkclient.h"

#include <QJsonObject>

BookCoverLoader& BookCoverLoader::instance()
{
    static BookCoverLoader instance;
    return instance;
}

QPixmap BookCoverLoader::cover(int bookId)
{
    if (bookId <= 0)
        return QPixmap();

    auto it = cache.find(bookId);
    if (it != cache.end())
        return it.value();

    QJsonObject data;
    data["book_id"] = bookId;

    QJsonObject response = NetworkClient::instance().sendRequest(RequestType::GetBookCover, data);

    QPixmap pix;
    if (response.value("status").toString() == "Success") {
        QByteArray bytes = QByteArray::fromBase64(
            response.value("data").toObject().value("cover_data").toString().toLatin1());
        pix.loadFromData(bytes);
    }


    cache.insert(bookId, pix);
    return pix;
}

void BookCoverLoader::clearCache()
{
    cache.clear();
}
