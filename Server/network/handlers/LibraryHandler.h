#ifndef LIBRARYHANDLER_H
#define LIBRARYHANDLER_H

#include "BaseHandler.h"
#include "../../../Common/protocol.h"

class LibraryHandler : public BaseHandler
{
public:
    static QJsonObject handle(RequestType type,
                              const QJsonObject &data,
                              ClientHandler *client);

private:
    static QJsonObject handleGetPurchasedBooks(ClientHandler *client);
    static QJsonObject handleUpdateLastReadPage(const QJsonObject &data, ClientHandler *client);
    static QJsonObject handleGetLastReadPage(const QJsonObject &data, ClientHandler *client);

    static QJsonObject handleSaveBook(const QJsonObject &data, ClientHandler *client);
    static QJsonObject handleRemoveSavedBook(const QJsonObject &data, ClientHandler *client);
    static QJsonObject handleGetSavedBooks(ClientHandler *client);

    static QJsonObject handleGetShelves(ClientHandler *client);
    static QJsonObject handleAddShelf(const QJsonObject &data, ClientHandler *client);
    static QJsonObject handleRemoveShelf(const QJsonObject &data, ClientHandler *client);
    static QJsonObject handleRenameShelf(const QJsonObject &data, ClientHandler *client);
    static QJsonObject handleAddBookToShelf(const QJsonObject &data, ClientHandler *client);
    static QJsonObject handleRemoveBookFromShelf(const QJsonObject &data, ClientHandler *client);
    static QJsonObject handleMoveBookBetweenShelves(const QJsonObject &data, ClientHandler *client);
};

#endif