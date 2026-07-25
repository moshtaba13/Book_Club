#include "UserManager.h"
#include "AuthManager.h"

UserManager& UserManager::instance() {
    static UserManager instance;
    return instance;
}

bool UserManager::blockUser(int userId) {
    return MemberRepository::instance().updateBlockStatus(userId, true);
}

bool UserManager::unblockUser(int userId) {
     return MemberRepository::instance().updateBlockStatus(userId, false);
}

bool UserManager::updateUsername(int userId,const QString &newUsername)
{
    if (MemberRepository::instance().usernameExists(newUsername))
        return false;

    return MemberRepository::instance().updateUsername(userId, AuthManager::instance().encrypt(newUsername));
}

bool UserManager::updatePassword(int userId, const QString &oldPassword, const QString &newPassword)
{
    auto member = MemberRepository::instance().findById(userId);
    if (!member)
        return false;

    QString oldsalt = MemberRepository::instance().getSalt(member->getUsername());
    if (member->getPassword() != AuthManager::instance().hashPassword(oldPassword, oldsalt))
        return false;

    QString newSalt = AuthManager::instance().generateSalt();

    return MemberRepository::instance().updatePassword(
        userId, 
        AuthManager::instance().hashPassword(newPassword, newSalt),
        newSalt
    );
}

bool UserManager::updateFavoriteGenres(int userId, const QVector<genre> &genres) 
{
    if (MemberRepository::instance().isFirstLogin(userId))
        if(!MemberRepository::instance().updateFirstLogin(userId, false))
            return false;
    return MemberRepository::instance().updateFavoriteGenres(userId, genres);
}
