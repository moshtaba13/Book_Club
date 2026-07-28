#ifndef ADMINHANDLER_H
#define ADMINHANDLER_H

#include "BaseHandler.h"
#include "../../../Common/protocol.h"

class AdminHandler : public BaseHandler
{
private:
    static QJsonObject handleGetAllMembers(ClientHandler *client);
    static QJsonObject handleGetAllUsers(ClientHandler *client);
    static QJsonObject handleGetAllPublishers(ClientHandler *client);
    static QJsonObject handleSearchUsers(const QJsonObject &data, ClientHandler *client);
    static QJsonObject handleGetBlockedUsers(ClientHandler *client);
    static QJsonObject handleGetActiveUsers(ClientHandler *client);
    static QJsonObject handleBlockUser(const QJsonObject &data, ClientHandler *client);
    static QJsonObject handleUnblockUser(const QJsonObject &data, ClientHandler *client);
    static QJsonObject handleDeleteUser(const QJsonObject &data, ClientHandler *client);

    static QJsonObject handleGetAllBooksAdmin(ClientHandler *client);
    static QJsonObject handleDeleteBookAdmin(const QJsonObject &data, ClientHandler *client);
    static QJsonObject handleEditBookAdmin(const QJsonObject &data, ClientHandler *client);
    static QJsonObject handleDeleteReviewAdmin(const QJsonObject &data, ClientHandler *client);
    
    public:
    static QJsonObject handle(RequestType type,
                              const QJsonObject &data,
                              ClientHandler *client);
};

#endif