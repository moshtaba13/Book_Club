#include "UserManager.h"

#include <QJsonArray>

#include "networkclient.h"
#include "modelserializer.h"
#include "protocol.h"

UserManager::UserManager() {}

bool UserManager::registerUser(const QString &username, const QString &password,
                               const QString &securityAnswer, QString &errorOut)
{
    QJsonObject data;
    data["username"] = username;
    data["password"] = password;
    data["security_answer"] = securityAnswer;

    QJsonObject response = NetworkClient::instance().sendRequest(RequestType::Register, data);
    if (response.value("status").toString() != "Success") {
        errorOut = response.value("message").toString("Registration failed.");
        return false;
    }
    return true;
}

bool UserManager::resetPassword(const QString &username, const QString &securityAnswer,
                                const QString &newPassword, QString &errorOut)
{
    QJsonObject data;
    data["username"] = username;
    data["security_answer"] = securityAnswer;
    data["new_password"] = newPassword;

    QJsonObject response = NetworkClient::instance().sendRequest(RequestType::ResetPassword, data);
    if (response.value("status").toString() != "Success") {
        errorOut = response.value("message").toString("Could not reset the password.");
        return false;
    }
    return true;
}

bool UserManager::changeUsername(const QString &newUsername, QString &errorOut)
{
    QJsonObject data;
    data["username"] = newUsername;

    QJsonObject response = NetworkClient::instance().sendRequest(RequestType::ChangeUsername, data);
    if (response.value("status").toString() != "Success") {
        errorOut = response.value("message").toString("Could not change the username.");
        return false;
    }
    return true;
}

bool UserManager::changePassword(const QString &oldPassword, const QString &newPassword, QString &errorOut)
{
    QJsonObject data;
    data["old_password"] = oldPassword;
    data["new_password"] = newPassword;

    QJsonObject response = NetworkClient::instance().sendRequest(RequestType::ChangePassword, data);
    if (response.value("status").toString() != "Success") {
        errorOut = response.value("message").toString("Could not change the password.");
        return false;
    }
    return true;
}

bool UserManager::updateFavoriteGenres(const QVector<genre> &genres, QString &errorOut)
{
    QJsonArray genresArray;
    for (const genre &g : genres)
        genresArray.append(genreToString(g));

    QJsonObject data;
    data["genres"] = genresArray;

    QJsonObject response = NetworkClient::instance().sendRequest(RequestType::UpdateFavoriteGenres, data);
    if (response.value("status").toString() != "Success") {
        errorOut = response.value("message").toString("Could not update favorite genres.");
        return false;
    }
    return true;
}

QVector<User> UserManager::getAllUsers(QString &errorOut)
{
    QVector<User> users;
    QJsonObject response = NetworkClient::instance().sendRequest(RequestType::GetAllUsers);

    if (response.value("status").toString() != "Success") {
        errorOut = response.value("message").toString("Could not load users.");
        return users;
    }

    for (const QJsonValue &v : response.value("data").toObject().value("users").toArray())
        users.append(ModelSerializer::userFromJson(v.toObject()));

    return users;
}

bool UserManager::setUserBlocked(int userId, bool blocked, QString &errorOut)
{
    QJsonObject data;
    data["user_id"] = userId;

    QJsonObject response = NetworkClient::instance().sendRequest(
        blocked ? RequestType::BlockUser : RequestType::UnblockUser, data);

    if (response.value("status").toString() != "Success") {
        errorOut = response.value("message").toString("Could not update the user's status.");
        return false;
    }
    return true;
}

bool UserManager::deleteUser(int userId, QString &errorOut)
{
    QJsonObject data;
    data["user_id"] = userId;

    QJsonObject response = NetworkClient::instance().sendRequest(RequestType::DeleteUser, data);
    if (response.value("status").toString() != "Success") {
        errorOut = response.value("message").toString("Could not delete the user.");
        return false;
    }
    return true;
}
