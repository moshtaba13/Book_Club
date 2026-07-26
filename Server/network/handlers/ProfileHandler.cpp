#include "ProfileHandler.h"
#include "../ClientHandler.h"
#include "../../managers/UserManager.h"


QJsonObject ProfileHandler::handleGetProfile(ClientHandler *client)
{
    if (!isLoggedIn(client))
        return unauthorized();

    return success(memberToJson(*client->currentUser()));
}

QJsonObject ProfileHandler::handleChangeUsername(const QJsonObject &data, ClientHandler *client)
{
    if (!isLoggedIn(client))
        return unauthorized();

    QString newUsername = data["username"].toString();

    if (newUsername.isEmpty())
        return invalidData("Username cannot be empty.");

    if (!UserManager::instance().updateUsername(client->currentUser()->getId(), newUsername))
        return alreadyExists("This username has already been used.");

    client->currentUser()->setUsername(newUsername);

    return success();
}

QJsonObject ProfileHandler::handleChangePassword(const QJsonObject &data, ClientHandler *client)
{
    if (!isLoggedIn(client))
        return unauthorized();

    QString oldPassword = data["old_password"].toString();
    QString newPassword = data["new_password"].toString();

    if (newPassword.isEmpty())
        return invalidData("New password cannot be empty.");

    if (!UserManager::instance().updatePassword(client->currentUser()->getId(), oldPassword, newPassword))
        return failure("The old password is incorrect.");

    return success();
}

QJsonObject ProfileHandler::handleUpdateFavoriteGenres(const QJsonObject &data,ClientHandler *client)
{
    if (!isUser(client))
        return unauthorized();

    QJsonArray genresArray = data["genres"].toArray();

    if (genresArray.size() > 3)
        return failure("A maximum of 3 genres are allowed.");

    if (genresArray.isEmpty())
        return failure("At least 1 genre must be selected.");

    QVector<genre> genres;
    for (const QJsonValue &v : genresArray)
        genres.append(stringToGenre(v.toString()));

    if (!UserManager::instance().updateFavoriteGenres(client->currentUser()->getId(), genres))
        return failure("Error updating genres");

    return success();
}

QJsonObject ProfileHandler::handle(RequestType type, const QJsonObject &data, ClientHandler *client)
{
    switch (type) {
    case RequestType::GetProfile:
        return handleGetProfile(client);
    case RequestType::ChangeUsername:
        return handleChangeUsername(data, client);
    case RequestType::ChangePassword:
        return handleChangePassword(data, client);
    case RequestType::UpdateFavoriteGenres:
        return handleUpdateFavoriteGenres(data, client);
    default:
        return failure("Unknown profile request");
    }
}