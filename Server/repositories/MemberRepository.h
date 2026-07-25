#ifndef MEMBERREPOSITORY_H
#define MEMBERREPOSITORY_H

#include <QVector>
#include <memory>

#include "../Database.h"
#include "../../Common/users/Member.h"
#include "../../Common/users/User.h"
#include "../../Common/users/Publisher.h"
#include "../../Common/users/Admin.h"
#include "../../Common/protocol.h"

using namespace std;

class MemberRepository
{
private:
    MemberRepository() = default;
    MemberRepository(const MemberRepository& ) = delete;
    MemberRepository& operator=(const MemberRepository& ) = delete;
    shared_ptr<Member> mapToMember(QSqlQuery &query);
public:
    static MemberRepository& instance();

    bool save(Member &member, const QString &salt);

    shared_ptr<Member> findById(int id);
    shared_ptr<Member> findByUsername(const QString &username);
    QVector<shared_ptr<Member>> findAll();
    QVector<shared_ptr<Member>> findAllUsers();
    QVector<shared_ptr<Member>> findAllPublishers();
    QVector<shared_ptr<Member>> findBlockedUsers();
    QVector<shared_ptr<Member>> findActiveUsers();
    QVector<shared_ptr<Member>> searchByUsername(const QString &keyword);

    bool updateUsername(int userId, const QString &newUsername);
    bool updatePassword(int userId, const QString &newPassword, const QString &newsalt);
    bool updateBlockStatus(int userId, bool blocked);
    bool updateFavoriteGenres(int userId, const QVector<genre> &genres);
    bool updateFirstLogin(int userId, bool isFirst);
    bool updateTotalRevenue(int publisherId, double revenue);

    bool remove(int id);
    bool usernameExists(const QString &username);
    bool isBlocked(const QString &username);
    bool isFirstLogin(int userId);

    QString getSalt(const QString &username);

};

#endif