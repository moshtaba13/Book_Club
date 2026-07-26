#include "UserManager.h"

UserManager::UserManager() {}

bool UserManager::usernameExists(const QString &username) const
{
    for (const User &u : registeredUsers) {
        if (u.getUsername() == username) {
            return true;
        }
    }
    return false;
}

bool UserManager::registerUser(const User &newUser)
{
    if (usernameExists(newUser.getUsername())) {
        return false;
    }
    registeredUsers.append(newUser);
    return true;
}

bool UserManager::authenticate(const QString &username, const QString &password, User &foundUser)
{
    for (const User &u : registeredUsers) {
        if (u.getUsername() == username && u.getPassword() == password) {
            foundUser = u;
            return true;
        }
    }
    return false;
}
bool UserManager::getSecurityAnswer(const QString &username, QString &answerOut)
{
    for (const User &u : registeredUsers) {
        if (u.getUsername() == username) {
            answerOut = u.getSecurityAnswer();
            return true;
        }
    }
    return false;
}

bool UserManager::resetPassword(const QString &username, const QString &securityAnswer, const QString &newPassword)
{
    for (int i = 0; i < registeredUsers.size(); ++i) {
        if (registeredUsers[i].getUsername() == username &&
            registeredUsers[i].getSecurityAnswer() == securityAnswer) {
            registeredUsers[i].setPassword(newPassword);
            return true;
        }
    }
    return false;
}
bool UserManager::isUsernameTakenByAnotherUser(const QString &username, int excludeUserId) const
{
    for (const User &u : registeredUsers) {
        if (u.getUsername() == username && u.getId() != excludeUserId) {
            return true;
        }
    }
    return false;
}
bool UserManager::updateUser(const User &updatedUser)
{
    for (int i = 0; i < registeredUsers.size(); ++i) {
        if (registeredUsers[i].getId() == updatedUser.getId()) {
            registeredUsers[i] = updatedUser;
            return true;
        }
    }
    return false;
}
QVector<User> UserManager::getAllUsers() const
{
    return registeredUsers;
}

bool UserManager::setUserBlocked(int userId, bool blocked)
{
    for (int i = 0; i < registeredUsers.size(); ++i) {
        if (registeredUsers[i].getId() == userId) {
            registeredUsers[i].setBlocked(blocked);
            return true;
        }
    }
    return false;
}

bool UserManager::deleteUser(int userId)
{
    for (int i = 0; i < registeredUsers.size(); ++i) {
        if (registeredUsers[i].getId() == userId) {
            registeredUsers.removeAt(i);
            return true;
        }
    }
    return false;
}