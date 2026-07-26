#ifndef BOOKHANDLER_H
#define BOOKHANDLER_H

#include "BaseHandler.h"
#include "../../../Common/Protocol.h"

class BookHandler : public BaseHandler
{
private:
    static QJsonObject handleGetAllBooks();
    static QJsonObject handleGetBooksByGenre(const QJsonObject &data);
    static QJsonObject handleGetBooksByPublisher(const QJsonObject &data, ClientHandler *client);
    static QJsonObject handleSearchBooks(const QJsonObject &data);
    static QJsonObject handleGetBookDetails(const QJsonObject &data);
    static QJsonObject handleAddBook(const QJsonObject &data, ClientHandler *client);
    static QJsonObject handleEditBook(const QJsonObject &data, ClientHandler *client);
    static QJsonObject handleInActiveBook(const QJsonObject &data, ClientHandler *client);
    static QJsonObject handleRestoreBook(const QJsonObject &data, ClientHandler *client);
    static QJsonObject handleApplyDiscount(const QJsonObject &data, ClientHandler *client);
    static QJsonObject handleAddReview(const QJsonObject &data, ClientHandler *client);
    static QJsonObject handleEditReview(const QJsonObject &data, ClientHandler *client);
    static QJsonObject handleDeleteReview(const QJsonObject &data, ClientHandler *client);
    static QJsonObject handleGetReviews(const QJsonObject &data);
    static QJsonObject handleGetTopSellingBooks();
    static QJsonObject handleGetLeastSellingBooks();
    static QJsonObject handleGetNewestBooks();
    static QJsonObject handleGetMostRatedBooks();
    static QJsonObject handleGetFreeBooks();
    static QJsonObject handleGetRecommendedBooks(ClientHandler *client);
    static QJsonObject handleGetBookFile(const QJsonObject &data, ClientHandler *client);
    static QJsonObject handleGetBookCover(const QJsonObject &data, ClientHandler *client);

    public:
    static QJsonObject handle(RequestType type,
                              const QJsonObject &data,
                              ClientHandler *client);
};

#endif