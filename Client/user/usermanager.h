#ifndef USERMANAGER_H
#define USERMANAGER_H

#include <QVector>
#include <QString>
#include "User.h"


class UserManager
{
public:
    UserManager();

    bool registerUser(const QString &username, const QString &password,
                      const QString &securityAnswer, QString &errorOut);
    bool resetPassword(const QString &username, const QString &securityAnswer,
                       const QString &newPassword, QString &errorOut);

    bool changeUsername(const QString &newUsername, QString &errorOut);
    bool changePassword(const QString &oldPassword, const QString &newPassword, QString &errorOut);
    bool updateFavoriteGenres(const QVector<genre> &genres, QString &errorOut);

    QVector<User> getAllUsers(QString &errorOut);
    bool setUserBlocked(int userId, bool blocked, QString &errorOut);
    bool deleteUser(int userId, QString &errorOut);
};

#endif // USERMANAGER_H
