#ifndef AUTHHANDLER_H
#define AUTHHANDLER_H

#include "BaseHandler.h"
#include "../../../Common/Protocol.h"

class AuthHandler : public BaseHandler
{
private:
    static QJsonObject handleLogin(const QJsonObject &data, ClientHandler *client);
    static QJsonObject handleRegister(const QJsonObject &data);
    static QJsonObject handleRegisterPublisher(const QJsonObject &data);
    static QJsonObject handleResetPassword(const QJsonObject &data);
    static QJsonObject handleLogout(ClientHandler *client);
public:
    static QJsonObject handle(RequestType type, const QJsonObject &data, ClientHandler *client);
};

#endif