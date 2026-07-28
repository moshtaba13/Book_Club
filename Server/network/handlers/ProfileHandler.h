#ifndef PROFILEHANDLER_H
#define PROFILEHANDLER_H

#include "BaseHandler.h"
#include "../../../Common/protocol.h"

class ProfileHandler : public BaseHandler
{
private:
    static QJsonObject handleGetProfile(ClientHandler *client);
    static QJsonObject handleChangeUsername(const QJsonObject &data, ClientHandler *client);
    static QJsonObject handleChangePassword(const QJsonObject &data, ClientHandler *client);
    static QJsonObject handleUpdateFavoriteGenres(const QJsonObject &data, ClientHandler *client);

public:
    static QJsonObject handle(RequestType type,
                              const QJsonObject &data,
                              ClientHandler *client);

};

#endif