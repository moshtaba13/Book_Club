#ifndef USERMANAGER_H
#define USERMANAGER_H

#include <QVector>
#include <memory>

#include "../repositories/MemberRepository.h"
#include "../repositories/NotificationRepository.h"
#include "../../Common/users/Member.h"

using namespace std;

class UserManager
{
private:
    UserManager() = default;
    UserManager(const UserManager&) = delete;
    UserManager& operator=(const UserManager&) = delete;

public:
    static UserManager& instance();

    bool blockUser(int userId);
    bool unblockUser(int userId);

    bool updateUsername(int userId, const QString &newUsername);
    bool updatePassword(int userId, const QString &oldPassword,const QString &newPassword);
    bool updateFavoriteGenres(int userId, const QVector<genre> &genres);

};

#endif