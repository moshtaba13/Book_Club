#include "MemberRepository.h"
#include <QJsonDocument>
#include <QJsonArray>
#include <QDateTime>

#include "../managers/AuthManager.h"

shared_ptr<Member> MemberRepository::mapToMember(QSqlQuery &query)
{
    QString role = query.value("role").toString();
    int id = query.value("id").toInt();
    QString username = AuthManager::instance().decrypt(query.value("username").toString());
    QString password = query.value("password").toString();
    QString secAnswer= query.value("security_answer").toString();

    shared_ptr<Member> member;

    if (role == "User") {
        auto user = make_shared<User>(username, password, secAnswer);
        user->setId(id);

        QString genresJson = query.value("favorite_genres").toString();
        QJsonArray genresArray = QJsonDocument::fromJson(genresJson.toUtf8()).array();

        QVector<genre> genres;
        for (const QJsonValue &v : genresArray)
            genres.append(stringToGenre(v.toString()));
        user->setFavoriteGenres(genres);

        user->setFirstLogin(query.value("is_first_login").toInt() == 1);

        QSqlQuery countQuery = Database::instance().createQuery();
        countQuery.prepare("SELECT COUNT(*) FROM purchases WHERE user_id = :uid");
        countQuery.bindValue(":uid", id);
        countQuery.exec();
    
        if (countQuery.next())
            user->setPurchaseCount(countQuery.value(0).toInt());

        member = user;

    } 
    else if (role == "Publisher") {
        auto publisher = make_shared<Publisher>(username, password, secAnswer);
        publisher->setId(id);
        publisher->setTotalRevenue(query.value("total_revenue").toDouble());
        member = publisher;

    } 
    else if (role == "Admin") {
        auto admin = make_shared<Admin>(username, password, secAnswer);
        admin->setId(id);
        member = admin;
    }

    if (member) {
        member->setBlocked(query.value("is_blocked").toInt() == 1);
        member->setRegisterDate(QDateTime::fromString(query.value("register_date").toString(),Qt::ISODate));
    }

    return member;
}

MemberRepository& MemberRepository::instance() {
    static MemberRepository instance;
    return instance;
}

bool MemberRepository::save(Member &member, const QString& salt)
{
    QSqlQuery query = Database::instance().createQuery();
    query.prepare(
        "INSERT INTO members "
        "(username, password, salt, role,"
        "security_answer, is_blocked, is_first_login,  "
        "favorite_genres, total_revenue, register_date) "
        "VALUES "
        "(:username, :password, :salt,   :role, "
        ":security_answer, :is_blocked, :is_first_login, "
        ":favorite_genres, :total_revenue, :register_date)"
    );

    query.bindValue(":username", member.getUsername());
    query.bindValue(":password", member.getPassword());
    query.bindValue(":salt", salt);
    query.bindValue(":role", member.role());
    query.bindValue(":security_answer", member.getSecurityAnswer());
    query.bindValue(":is_blocked", member.isBlocked() ? 1 : 0);
    query.bindValue(":register_date", QDateTime::currentDateTime().toString(Qt::ISODate));

    if (member.role() == "User") {
        User* user = dynamic_cast<User*>(&member);
        if (user) {
            QJsonArray genresArray;
            for (const genre &g : user->getfavoriteGenres())
                genresArray.append(genreToString(g));

            query.bindValue(":favorite_genres", QJsonDocument(genresArray).toJson(QJsonDocument::Compact));
            query.bindValue(":is_first_login", user->isFirstLogin() ? 1 : 0);
        }
    } 
    else {
        query.bindValue(":favorite_genres", "[]");
        query.bindValue(":is_first_login",  0);
    }

    if (member.role() == "Publisher") {
        Publisher* pub = dynamic_cast<Publisher*>(&member);
        query.bindValue(":total_revenue", pub ? pub->getTotalRevenue() : 0.0);
    } 
    else {
        query.bindValue(":total_revenue", 0.0);
    }

    if (!query.exec()) {
        qWarning() << "MemberRepository::save error:"<< query.lastError().text();
        return false;
    }

    member.setId(query.lastInsertId().toInt());
    return true;
}

shared_ptr<Member> MemberRepository::findById(int id) {
    QSqlQuery query = Database::instance().createQuery();
    query.prepare("SELECT * FROM members WHERE id = :id");
    query.bindValue(":id", id);

    if (!query.exec() || !query.next())
        return nullptr;

    return mapToMember(query);
}

shared_ptr<Member> MemberRepository::findByUsername(const QString &username) {
    QSqlQuery query = Database::instance().createQuery();
    query.prepare("SELECT * FROM members WHERE username = :username");
    query.bindValue(":username", AuthManager::instance().encrypt(username));

    if (!query.exec() || !query.next())
        return nullptr;

    return mapToMember(query);
}

QVector<shared_ptr<Member>> MemberRepository::findAll() {
    QVector<shared_ptr<Member>> members;
    QSqlQuery query = Database::instance().createQuery();

    if (!query.exec("SELECT * FROM members"))
        return members;

    while (query.next())
        members.append(mapToMember(query));

    return members;
}

QVector<shared_ptr<Member>> MemberRepository::findAllUsers() {
    QVector<shared_ptr<Member>> users;
    QSqlQuery query = Database::instance().createQuery();
    query.prepare("SELECT * FROM members WHERE role = 'User'");

    if (!query.exec())
        return users;

    while (query.next())
        users.append(mapToMember(query));

    return users;
}

QVector<shared_ptr<Member>> MemberRepository::findAllPublishers() {
    QVector<shared_ptr<Member>> publishers;
    QSqlQuery query = Database::instance().createQuery();
    query.prepare("SELECT * FROM members WHERE role = 'Publisher'");

    if (!query.exec())
        return publishers;

    while (query.next())
        publishers.append(mapToMember(query));

    return publishers;
}

QVector<shared_ptr<Member>> MemberRepository::searchByUsername(const QString &keyword)
{
    QVector<shared_ptr<Member>> members;
    QSqlQuery query = Database::instance().createQuery();
    query.prepare("SELECT * FROM members WHERE username LIKE :keyword");
    query.bindValue(":keyword", AuthManager::instance().encrypt(keyword));

    if (!query.exec())
        return members;

    while (query.next())
        members.append(mapToMember(query));

    return members;
}

QVector<shared_ptr<Member>> MemberRepository::findBlockedUsers()
{
    QVector<shared_ptr<Member>> members;
    QSqlQuery query = Database::instance().createQuery();
    query.prepare("SELECT * FROM members WHERE is_blocked = 1");

    if (!query.exec())
        return members;

    while (query.next())
        members.append(mapToMember(query));

    return members;
}

QVector<shared_ptr<Member>> MemberRepository::findActiveUsers()
{
    QVector<shared_ptr<Member>> members;
    QSqlQuery query = Database::instance().createQuery();
    query.prepare("SELECT * FROM members WHERE is_blocked = 0");

    if (!query.exec())
        return members;

    while (query.next())
        members.append(mapToMember(query));

    return members;
}

bool MemberRepository::updateUsername(int userId, const QString &newUsername) {
    QSqlQuery query = Database::instance().createQuery();
    query.prepare("UPDATE members SET username = :username WHERE id = :id");

    query.bindValue(":username", AuthManager::instance().encrypt(newUsername));
    query.bindValue(":id", userId);

    if (!query.exec()) {
        qWarning() << "MemberRepository::update error:" << query.lastError().text();
        return false;
    }
    return true;
}

bool MemberRepository::updatePassword(int userId, const QString &newPassword, const QString &newsalt)
{
    QSqlQuery query = Database::instance().createQuery();
    query.prepare("UPDATE members SET"
                  "password = :password,"
                  "salt = :salt"
                  "WHERE id = :id"
                );
    query.bindValue(":password", newPassword);
    query.bindValue(":salt", newsalt);
    query.bindValue(":id", userId);

    if (!query.exec()) {
        qWarning() << "UserRepository::updatePassword error:" << query.lastError().text();
        return false;
    }
    return true;
}

bool MemberRepository::updateBlockStatus(int userId, bool blocked) {
    QSqlQuery query = Database::instance().createQuery();
    query.prepare("UPDATE members SET is_blocked = :blocked WHERE id = :id");

    query.bindValue(":blocked", blocked ? 1 : 0);
    query.bindValue(":id",      userId);

    if (!query.exec()) {
        qWarning() << "UserRepository::updateBlockStatus error:" << query.lastError().text();
        return false;
    }
    return true;
}

bool MemberRepository::updateFavoriteGenres(int userId, const QVector<genre> &genres) {
    QJsonArray genresArray;
    for (const genre &g : genres)
        genresArray.append(genreToString(g));

    QSqlQuery query = Database::instance().createQuery();
    query.prepare("UPDATE members SET favorite_genres = :genres WHERE id = :id");
    query.bindValue(":genres", QJsonDocument(genresArray).toJson(QJsonDocument::Compact));
    query.bindValue(":id", userId);

    if (!query.exec()) {
        qWarning() << "UserRepository::updateFavoriteGenres error:"<< query.lastError().text();
        return false;
    }
    return true;
}

bool MemberRepository::updateFirstLogin(int userId, bool isFirst) {
    QSqlQuery query = Database::instance().createQuery();
    query.prepare("UPDATE members SET is_first_login = :first WHERE id = :id");
    query.bindValue(":first", isFirst ? 1 : 0);
    query.bindValue(":id", userId);

    if (!query.exec()) {
        qWarning() << "UserRepository::updateFirstLogin error:" << query.lastError().text();
        return false;
    }
    return true;
}

bool MemberRepository::updateTotalRevenue(int publisherId, double revenue)
{
    QSqlQuery query = Database::instance().createQuery();
    query.prepare("UPDATE members SET total_revenue = :revenue WHERE id = :id");
    query.bindValue(":revenue", revenue);
    query.bindValue(":id",      publisherId);

    if (!query.exec()) {
        qWarning() << "UserRepository::updateTotalRevenue error:"<< query.lastError().text();
        return false;
    }
    return true;
}

bool MemberRepository::remove(int id)
{
    QSqlQuery query = Database::instance().createQuery();
    query.prepare("DELETE FROM members WHERE id = :id");
    query.bindValue(":id", id);

    if (!query.exec()) {
        qWarning() << "UserRepository::remove error:" << query.lastError().text();
        return false;
    }
    return true;
}

bool MemberRepository::usernameExists(const QString &username) {
    QSqlQuery query = Database::instance().createQuery();
    query.prepare("SELECT id FROM members WHERE username = :username");
    query.bindValue(":username", AuthManager::instance().encrypt(username));
    query.exec();
    return query.next();
}

bool MemberRepository::isBlocked(const QString &username) {
    QSqlQuery query = Database::instance().createQuery();
    query.prepare("SELECT is_blocked FROM members WHERE username = :username");
    query.bindValue(":username", AuthManager::instance().encrypt(username));
    query.exec();

    if (query.next())
        return query.value("is_blocked").toInt() == 1;
    return false;
}

bool MemberRepository::isFirstLogin(int userId) {
    QSqlQuery query = Database::instance().createQuery();
    query.prepare("SELECT is_first_login FROM members WHERE id = :id");
    query.bindValue(":id", userId);
    query.exec();

    if (query.next())
        return query.value("is_first_login").toInt() == 1;
    return false;
}

QString MemberRepository::getSalt(const QString &username) {
    QSqlQuery query = Database::instance().createQuery();
    query.prepare("SELECT salt FROM members WHERE username = :username");
    query.bindValue(":username", AuthManager::instance().encrypt(username));
    query.exec();

    if (query.next())
        return query.value("salt").toString();
    return QString();
}