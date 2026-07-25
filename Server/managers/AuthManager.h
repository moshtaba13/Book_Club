#ifndef AUTHMANAGER_H
#define AUTHMANAGER_H

#include <QString>
#include <memory>

#include "../repositories/MemberRepository.h"
#include "../../Common/users/User.h"
#include "../../Common/users/Publisher.h"
#include "../../Common/users/Admin.h"

using namespace std;

class AuthManager
{
private:
    AuthManager() = default;
    AuthManager(const AuthManager&) = delete;
    AuthManager& operator=(const AuthManager&) = delete;

    const QString key = "BookClubSecretKey2026";
public:
    static AuthManager& instance();

    bool registerUser(const QString &username, const QString &password, const QString &securityAnswer);
    bool registerPublisher(const QString &username, const QString &password, const QString &securityAnswer);

    shared_ptr<Member> login(const QString &username, const QString &password);

    bool verifySecurityAnswer(const QString &username, const QString &answer);
    bool resetPassword(const QString &username, const QString &newPassword);
    
    QString generateSalt();
    QString hashPassword(const QString &password, const QString &salt);

    QString encrypt(const QString &text);
    QString decrypt(const QString &encrypted);

};

#endif