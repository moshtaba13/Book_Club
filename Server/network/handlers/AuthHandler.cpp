#include "AuthHandler.h"
#include "../ClientHandler.h"
#include "../../managers/AuthManager.h"


QJsonObject AuthHandler::handleLogin(const QJsonObject &data, ClientHandler *client)
{
    QString username = data["username"].toString();
    QString password = data["password"].toString();
    
    if (client->currentUser()->isBlocked())
        return failure("Your account is blocked");
    auto member = AuthManager::instance().login(username, password);
    if (!member)
        return notFound("invalid password or username");

    client->setCurrentUser(member);
    return success(memberToJsonFull(*member));
}

QJsonObject AuthHandler::handleRegister(const QJsonObject &data)
{
    QString username = data["username"].toString();
    QString password = data["password"].toString();
    QString securityAnswer = data["security_answer"].toString();

    if (!AuthManager::instance().registerUser(username, password, securityAnswer))
        return alreadyExists("this username already taken");

    return success();
}

QJsonObject AuthHandler::handleRegisterPublisher(const QJsonObject &data)
{
    QString username = data["username"].toString();
    QString password = data["password"].toString();
    QString securityAnswer = data["security_answer"].toString();

    if (!AuthManager::instance().registerPublisher(username, password, securityAnswer))
        return alreadyExists("this username already taken");

    return success();
}

QJsonObject AuthHandler::handleResetPassword(const QJsonObject &data)
{
    QString username = data["username"].toString();
    QString securityAnswer = data["security_answer"].toString();
    QString newPassword = data["new_password"].toString();

    if (!AuthManager::instance().verifySecurityAnswer(username, securityAnswer))
        return failure("wrong anser");

    if (!AuthManager::instance().resetPassword(username, newPassword))
        return failure("Error changing password");

    return success();
}

QJsonObject AuthHandler::handleLogout(ClientHandler *client)
{
    client->setCurrentUser(nullptr);
    return success();
}

QJsonObject AuthHandler::handle(RequestType type, const QJsonObject &data, ClientHandler *client)
{
    switch (type) {
    case RequestType::Login:
        return handleLogin(data, client);
    case RequestType::Register:
        return handleRegister(data);
    case RequestType::RegisterPublisher:
        return handleRegisterPublisher(data);
    case RequestType::ResetPassword:
        return handleResetPassword(data);
    case RequestType::Logout:
        return handleLogout(client);
    default:
        return failure("Unknown auth request");
    }
}