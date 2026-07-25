#include "AuthManager.h"
#include <QCryptographicHash>
#include <QUuid>

AuthManager& AuthManager::instance() {
    static AuthManager instance;
    return instance;
}

QString AuthManager::generateSalt() {
    return QUuid::createUuid().toString();
}

QString AuthManager::hashPassword(const QString &password, const QString &salt) {
    return QString(QCryptographicHash::hash((password + salt).toUtf8(), QCryptographicHash::Sha256).toHex());
}

QString AuthManager::encrypt(const QString &text)
{
    QByteArray data = text.toUtf8();
    QByteArray k  = key.toUtf8();

    for (int i = 0; i < data.size(); i++)
        data[i] = data[i] ^ k[i % key.size()];

    return QString::fromLatin1(data.toBase64());
}

QString AuthManager::decrypt(const QString &encrypted)
{
    QByteArray data = QByteArray::fromBase64(encrypted.toLatin1());
    QByteArray k  = key.toUtf8();

    for (int i = 0; i < data.size(); ++i)
        data[i] = data[i] ^ k[i % key.size()];

    return QString::fromUtf8(data);
}

bool AuthManager::registerUser(const QString &username, const QString &password, const QString &securityAnswer)
{
    if (MemberRepository::instance().usernameExists(username))
        return false;
    
    QString salt = generateSalt();

    User newUser(
        encrypt(username),       
        hashPassword(password, salt),   
        encrypt(securityAnswer)   
    );


    return MemberRepository::instance().save(newUser, salt);
}

bool AuthManager::registerPublisher(const QString &username, const QString &password, const QString &securityAnswer)
{
    if (MemberRepository::instance().usernameExists(username))
        return false;
    
    QString salt = generateSalt();

    Publisher newPublisher(
        encrypt(username),       
        hashPassword(password, salt),   
        encrypt(securityAnswer)   
    );

    return MemberRepository::instance().save(newPublisher, salt);
}

shared_ptr<Member> AuthManager::login(const QString &username, const QString &password)
{
    auto member = MemberRepository::instance().findByUsername(username);

    if (!member)
        return nullptr;

    if (member->getPassword() != hashPassword(password, MemberRepository::instance().getSalt(username)))
        return nullptr;

    return member;
}

bool AuthManager::verifySecurityAnswer(const QString &username, const QString &answer){
    auto member = MemberRepository::instance().findByUsername(username);

    if (!member)
        return false;

    return decrypt(member->getSecurityAnswer()) == answer;
}

bool AuthManager::resetPassword(const QString &username, const QString &newPassword)
{
    auto member = MemberRepository::instance().findByUsername(username);

    if (!member)
        return false;

    QString newSalt = generateSalt();

    return MemberRepository::instance().updatePassword(
        member->getId(),
        hashPassword(newPassword, newSalt),
        newSalt
    );
}